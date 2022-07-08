
/************************************************************************
 *
 *			                >>>>>>> BUFGRANUL2~ <<<<<<<
 *
 *						  multi-buffer enveloppe externe, 
 *                          multi-buffer son externe.
 *              continuite des grains lors d'un changement de buffer.
 *                         controle en float et/ou en audio
 * 					selection de buffer son par entree signal
 *                           ----------------------
 *                              GMEM 2002-2004
 *         Laurent Pottier / Loic Kessous / Charles Bascou / Leopold Frey
 *
 * -----------------------------------------------------------------------
 *
 * 
 * N.B. 
 *      * Pour compatibilite maximum Mac/PC :
 *        pas d'accents dans les commentaires svp
 *
 *      * Faites des commentaires !
 *
 ************************************************************************/
 
 /*** DSP implementation file ***/ 
 
#include "bufGranul2~.h"

static int x_sinc_table_offset[16] = {15360 ,14336 ,13312 ,12288 ,11264 ,10240 ,9216 ,8192 ,7168 ,6144 ,5120 ,4096 ,3072 ,2048 ,1024 ,0};

//***********************************************************************************//

void bufGranul_perform(t_bufGranul *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{

    t_double    *   in		= ins[0];
    t_double    *   t_begin = ins[1];
    t_double    *   t_transpos = ins[2];
    t_double    *   t_amp = ins[3];
    t_double    *   t_length = ins[4];
    t_double    *   t_pan = ins[5];
    t_double    *   t_dist = ins[6];
    t_double    *   t_sndbuf = ins[7];
      
    int n = sampleframes;

	
    int N = n;
    int xn = 0, p, hp;

	double srms = x->x_sr*0.001; // double for precision
	float val;
    double sigin, prevsigin,savein; // values of actual and previous input sample

	double beginind = x->x_begin*srms; // double for precision (this number can be very large)
	double lengthind = x->x_length*srms;
	
	// temporary buffer index variable
	int t_active_buf ; // buffer actif pour la crŽation si pas entrŽe bufnum signal ( x->x_active_buf peut ne plus tre valide )
	int t_bufnum;
	int t_bufchan;
    int t_bchan_offset = x->x_bchan_offset;
    int t_bchan_offset_mod;
	int t_active_env;
	float * t_bufsamples;
	float * t_envsamples;

    int i,j;
    long k,l,m,k_base;
	int nvoices = x->x_nvoices;
	
	int initTest;
	
	// variables grains
	int t_bufframes;
	
	double finc;
	double temp_sind;
	double source_findex, target_findex;
	int dsp_i_begin;
	int dsp_i_end;
		
    long loop_length;
    long loop_start, loop_end;
	
	int nech_process, nech_process_sat;
	
		//trick interp long index
	long base_lindex;
	long lindex;
	long lincr;
    
    double sinc_indexf;
    double fract_index;
    double hp_norm;

	
	long base_lindex_env;
	long lindex_env;
	long lincr_env;
	
	int buffer_index;
	int buffer_index_plus_1;
	int interp_table_index;
	
	t_linear_interp * interp_table = x->x_linear_interp_table;
    double * sinc_interp_table = x->x_sinc_interp_table;
    int sinc_phasei = SINC_TABLE_SIZE / 16;

    double * fade_table;
    
    double microtiming_coeff;
    
	// debug var
#ifdef PERF_DEBUG
    int n_voices_active;
#endif

   	sigin = x->x_sigin;
   	
   	
#ifdef PERF_DEBUG
	if(x->ask_poll)
	{
		_SETSYM( x->info_list, gensym("voices"));
        _SETLONG(x->info_list+1, x->x_nvoices_active);
		outlet_list(x->info,0l,2,x->info_list);
	}
#endif

	initTest = bufGranul_bufferinfos(x);
   	if(initTest == 0) goto zero;
	else
	{
		t_active_buf = buffer_check(x,x->x_active_buf);
		t_active_env = bufferenv_check(x,x->x_active_env);
		// %%%%%%%%%%     creation de grains   %%%%%%%%%%%%%%%%%
   			
   		p=nvoices;
   		
		// Dans le cas d'un grain declenche par un bang
		savein = in[0];
   		if(x->x_askfor)
   		{
			sigin = -1.;
			in[0] = 1.;
			x->x_askfor = 0;
   		}
		
		// Meme chose avec declenchement par zero-crossing sur tout le vecteur in, dans la limite fixee par nvoices
		while (n-- && p)
		{
			//-----signal zero-crossing ascendant declenche un grain----//
			prevsigin = sigin;
			sigin = in[xn];
			if (prevsigin <= 0 && sigin > 0) // creation d'un grain
			{
                microtiming_coeff = x->x_microtiming * prevsigin/(sigin - prevsigin);
                
				p = bufGranul_poly_assign_voice(x); // get free voice according to polymode
				
				if(p)
				{
    				x->x_voiceOn[p] = 1;
					x->x_sind[p] = x->Vbeg[p] = microtiming_coeff + (x->x_in2con) * t_begin[xn]*srms + (1 - x->x_in2con)* beginind;	// index dans buffer
					x->Vtranspos[p] = (x->x_in3con) * t_transpos[xn] + (1 - x->x_in3con) * x->x_transpos;			// valeur de pitch
					x->Vamp[p]		= (x->x_in4con) * t_amp[xn] + (1 - x->x_in4con) * x->x_amp;						// amplitude
					x->Vbuf[p] = (x->x_in8con) ? buffer_check(x,(int) t_sndbuf[xn] ) :  t_active_buf;		// numero du buffer son	
					x->Venv[p] = t_active_env;	// enveloppe active pour ce grain
     
					if(x->x_in5con)
					{
						if(t_length[xn]<0)
						{	
							x->Vlength[p]	= -t_length[xn]*srms;
							x->envinc[p]	= -1.*(float)(x->x_env_frames[t_active_env] - 2) / x->Vlength[p] ;
							x->envind[p]	= x->x_env_frames[t_active_env] - 1  + (1+microtiming_coeff) * x->envinc[p] ;
						}
                        else if (t_length[xn] == 0.) // whole buffer
                        {
                            x->Vlength[p] = x->x_buf_frames[x->Vbuf[p]];
                            x->envinc[p]    = (float)(x->x_env_frames[t_active_env] - 2) / x->Vlength[p] ;
                            x->envind[p]    = (1+microtiming_coeff) * x->envinc[p];
                        }
						else
						{
							x->Vlength[p]	= t_length[xn]*srms;
							x->envinc[p]	= (float)(x->x_env_frames[t_active_env] - 2) / x->Vlength[p] ;
                            x->envind[p]	=  (1+microtiming_coeff) * x->envinc[p] ;
						}
					}
					else
					{
                        if(lengthind == 0.)
                            x->Vlength[p] = x->x_buf_frames[x->Vbuf[p]];
                        else
                            x->Vlength[p] = lengthind;
						x->envinc[p] = x->x_env_dir * x->x_env_frames[t_active_env] / x->Vlength[p] ;
						if(x->x_env_dir < 0)
							x->envind[p] =  x->x_env_frames[t_active_env] - 1;
						else 
							x->envind[p] = 0.;
					}


					x->Vpan[p]		= (x->x_in6con) * t_pan[xn] + (1 - x->x_in6con) *  x->x_pan;						// pan
                    x->Vpan[p]      = MOD(x->Vpan[p],1);
					x->Vdist[p]		= (x->x_in7con) * t_dist[xn] + (1 - x->x_in7con) *  x->x_dist;					// distance
                    
                    x->Vloop[p] = x->x_loop;
                    x->Vloopstart[p] = x->x_loopstart;
                    x->Vloopend[p] = x->x_loopend;
          
                    panner(x->Vhp[p],x->x_nouts,MAX(x->Vpan[p],0),SAT(x->Vdist[p], 0., 1.));
                    
					x->x_ind[p] = 0;
					x->x_remain_ind[p] = (long) x->Vlength[p];
					x->x_delay[p] = xn;   // delay de declenchement dans le vecteur         	  
    					
				} else goto perform;
			}
			xn++ ; // boucle on incremente le pointeur dans le vecteur in et le delay de declenchement

		}
   		 
		// %%%%%%%%%%     fin creation     %%%%%%%%%%%%%%%%%
   		
   		perform :
		
		in[0] = savein;
   		//&&&&&&&&&&&&&  Boucle DSP  &&&&&&&&&&&&&
   		n = N;
   		j=0;
        
        for(hp = 0; hp < x->x_nouts; hp++ )
            memset(outs[hp],0,N*sizeof(double));
        
		for (i=0; i < NVOICES ; i++)
		{         
			//si la voix est en cours              
			if (x->x_voiceOn[i]>0) //&& x->x_ind[i] < x->Vlength[i] )
			{
				// si delay + grand que taille vecteur on passe ˆ voix suivante 
				if(x->x_delay[i] >= N)
				{
                    if(x->x_voiceOn[i]==3) // kill mode -> free voice
                        x->x_voiceOn[i]=0;
                    else
                        x->x_voiceOn[i]=2; // wait mode
					x->x_delay[i] -= N ;
					goto next;
				}
                
                if(x->x_voiceOn[i]==2) // waiting voice now ready for playing -> kill voices if poly exceeded
                {
                    bufGranul_poly_check_and_kill(x);
                    x->x_voiceOn[i]=1;
                }
                if(x->x_voiceOn[i]==3) // kill voice
                    fade_table=x->x_kill_fadeout+x->x_delay[i]; // decalage du pointeur de fade out par le delai
                //else if(x->x_ind[i]==0) // fadein
                //    fade_table=x->x_fadein;
                else
                    fade_table=x->x_unity_gain;
				
				// nb d'ech a caluler pour ce vecteur
				nech_process = MIN( (N - x->x_delay[i]) , x->x_remain_ind[i] );
				
				// Selon le buffer dans lequel on doit prendre le son x->Vbuf[i] (voir creation des grains)
				// Il se peut que le buffer associŽ au grain ne soit plus valide -> verification -> si non on prend buffer #0
				t_bufnum = x->x_buf_valid_index[x->Vbuf[i]];
				t_bufframes =  x->x_buf_frames[t_bufnum];
				t_bufsamples = x->x_buf_samples[t_bufnum];
				t_bufchan = x->x_buf_nchan[t_bufnum];
                t_bchan_offset_mod = t_bchan_offset % t_bufchan;
				
				t_envsamples = x->x_env_samples[x->Venv[i]];
				
				// pas d'increment en float
				finc = (x->Vtranspos[i]*x->x_buf_sronsr[t_bufnum]);

				// snd index source 
				source_findex = x->x_sind[i];
				// snd index cible en fin de boucle
				target_findex = source_findex + nech_process * finc;
				
				
				// nb ech a calculer different selon boucle ou pas
				if( !x->Vloop[i] )
				{
					// si index source hors des bornes -> kill grain
					if( source_findex >  t_bufframes || source_findex <  0 )
					{
						x->x_voiceOn[i] = 0;
						goto next;
					}
					
					// nb SATURE d'ech a calculer pour ce vecteur (0 < ... < frames) ou (startloop < ... < endloop)
					if(target_findex >  t_bufframes )
					{
						// on met la voix off
						x->x_voiceOn[i] = 0;
						// jusqu'a quel index on calcule
						nech_process_sat = (long)(nech_process * (t_bufframes - source_findex) /  (target_findex - source_findex));
						
						dsp_i_begin = x->x_delay[i];
						dsp_i_end = x->x_delay[i] + nech_process_sat;
						
						temp_sind = x->x_sind[i];
						base_lindex = (long) x->x_sind[i];
						lindex = interp_index_scale( x->x_sind[i] - (double) base_lindex );
						lincr  = interp_index_scale(finc);
					
					}else if( target_findex < 0 )
					{
						// on met la voix off
						x->x_voiceOn[i] = 0;
						// jusqu'a quel index on calcule   ATTENTION arrondi depassement
						nech_process_sat = (long)(nech_process * (source_findex) /  (source_findex - target_findex));
						
						dsp_i_begin = x->x_delay[i];
						dsp_i_end = x->x_delay[i] + nech_process_sat;
						
						temp_sind = x->x_sind[i];
						base_lindex = (long) x->x_sind[i];
						lindex = interp_index_scale( x->x_sind[i] - (double) base_lindex );
						lincr  = interp_index_scale(finc);
						
					}
					else 
					{
						nech_process_sat = nech_process;
						
						dsp_i_begin = x->x_delay[i];
						dsp_i_end = x->x_delay[i] + nech_process_sat;
						
						temp_sind = x->x_sind[i];
						base_lindex = (long) x->x_sind[i];
						lindex = interp_index_scale( x->x_sind[i] - (double) base_lindex );
						lincr  = interp_index_scale(finc);
                        
                        fract_index = x->x_sind[i] - (double) base_lindex;
                        
							
					}
				}
				else // if loop
				{
					/////////// LOOOP //////////
					nech_process_sat = nech_process;
					
					//target_findex = MOD(x->x_sind[i],x->x_buf_looplength[t_bufnum]);
					
					dsp_i_begin = x->x_delay[i];
					dsp_i_end = x->x_delay[i] + nech_process_sat;
					
                    if(x->Vloop[i] == 1){
                        loop_start = 0;
                        loop_end = t_bufframes;
                        loop_length = loop_end - loop_start;}
                    if(x->Vloop[i] == 2){
                        loop_start = SAT(x->Vloopstart[i]*srms,0,t_bufframes-1);
                        loop_end = SAT(x->Vloopend[i]*srms,loop_start+1,t_bufframes);
                        loop_length = loop_end - loop_start;}
					
					// calcul du vrai float index en ech de lecture
					temp_sind = x->x_sind[i];
					temp_sind -= loop_start;
			        temp_sind = loop_start + MOD(temp_sind,loop_length);  
					
					// calcul de l'index de lecture buffer en long
					base_lindex = (long) temp_sind;
					lindex = interp_index_scale( temp_sind - (double) base_lindex );
					lincr  = interp_index_scale(finc);					
					

				}
				
				
				// Enveloppe long index & incr calcul
				base_lindex_env = (long) x->envind[i];
				lindex_env = interp_index_scale( x->envind[i] - (double) base_lindex_env );
				lincr_env  = interp_index_scale(x->envinc[i]);
				
				x->envind[i] += nech_process_sat * x->envinc[i];
				
				
				
				//***********************************
				// CALCUL EFFECTIF DES ECHANTILLONS
				//***********************************
				
				if(x->Vloop[i])
					if( x->x_sinterp == 1 )  // loop + interp
						for(j= dsp_i_begin; j < dsp_i_end; j++)
						{
						// Lecture de l'enveloppe
							buffer_index = base_lindex_env + interp_get_int( lindex_env );
							interp_table_index = interp_get_table_index( lindex_env );
			      			x->x_env[i] = interp_table[interp_table_index].a * t_envsamples[buffer_index]
			      							+ interp_table[interp_table_index].b * t_envsamples[buffer_index + 1];
			      			
			      			lindex_env += lincr_env;
			      			
			      			//x->envind[i] += x->envinc[i]; // 2 REMOVE
							
							// Lecture de la forme d'onde
				    	  	buffer_index = base_lindex + interp_get_int( lindex );
				    	  	buffer_index -= loop_start;
							buffer_index = loop_start + MODI(buffer_index,loop_length);
							
							buffer_index_plus_1 = buffer_index + 1;
							//buffer_index_plus_1 = MODI(buffer_index_plus_1,x->x_buf_looplength[t_bufnum]);
							
							interp_table_index = interp_get_table_index( lindex );
			      			val = interp_table[interp_table_index].a * t_bufsamples[(buffer_index * t_bufchan) + t_bchan_offset_mod]
			      							+ interp_table[interp_table_index].b * t_bufsamples[(buffer_index_plus_1 * t_bufchan) + t_bchan_offset_mod];
			      			
			      			lindex += lincr;
			      			
							// calcul de la valeur env[i]*son[i]*amp[i]
							val = x->x_env[i] * val * x->Vamp[i] * (*fade_table++);
                            
                            // calcul du pan en fonction de Vhp1[i] et Vhp2[i]
                            for(hp = 0; hp < x->x_nouts; hp++ )
                                outs[hp][j] += x->Vhp[i][hp]*val;
                            

						}
                    else if ( x->x_sinterp == 2 ) // LOOP + interp SINC ///**************************************** ///
                    {
                        double highpass_fact = (finc > 1.)? 1/finc : 1.;
                        long norm_index = 0;
                        
                        for(j= dsp_i_begin; j < dsp_i_end; j++)
                        {
                            
                            // Lecture de l'enveloppe
                            buffer_index = base_lindex_env + interp_get_int( lindex_env );
                            interp_table_index = interp_get_table_index( lindex_env );
                            x->x_env[i] = interp_table[interp_table_index].a * t_envsamples[buffer_index]
                            + interp_table[interp_table_index].b * t_envsamples[buffer_index + 1];
                            
                            lindex_env += lincr_env;
                            
                            val = 0.;
                            long temp_iind = (long) temp_sind ;
                            fract_index = temp_sind - (double)temp_iind;
                            long fract_index_i = (double) (fract_index * (double)sinc_phasei);
                            
                            
                            
                            for(m=-7;m<9;m++)
                            {
                                k = temp_iind + m;
                                k-= loop_start;
                                k = MODI(k,loop_length);
                                k+= loop_start;
                                
                                val += x->x_blackman_table[x_sinc_table_offset[m+7]+fract_index_i] * x->x_sinc_interp_table[(long)(8192+ highpass_fact * (8192 - (x_sinc_table_offset[m+7]+fract_index_i)))] * t_bufsamples[(k * t_bufchan) + t_bchan_offset_mod];
                                //   printf( " index %f ",x->x_sinc_interp_table[x_sinc_table_offset[m+7]]);
                            }
                            
                            
                            temp_sind += finc;
                            
                            norm_index = (long)(highpass_fact*SINC_TABLE_SIZE)-1;
                            //printf("nindex %d\n",norm_index);
                            
                            hp_norm = x->x_sinc_norm_table[norm_index];
                            
                            
                            // calcul de la valeur env[i]*son[i]*amp[i]
                            val = x->x_env[i]*val*x->Vamp[i]*hp_norm*(*fade_table++);
                            
                            // calcul du pan en fonction de Vhp1[i] et Vhp2[i]
                            for(hp = 0; hp < x->x_nouts; hp++ )
                                outs[hp][j] += x->Vhp[i][hp]*val;
                            
                            
                        }
                    }
					else	// loop + non interp
						for(j= dsp_i_begin; j < dsp_i_end; j++)
						{
						// Lecture de l'enveloppe
							buffer_index = base_lindex_env + interp_get_int( lindex_env );
			      			x->x_env[i] = t_envsamples[buffer_index];
			      			
			      			lindex_env += lincr_env;
			      			
			      			//x->envind[i] += x->envinc[i]; // 2 REMOVE
							
							// Lecture de la forme d'onde
				    	  	buffer_index = base_lindex + interp_get_int( lindex );
				    	  	buffer_index -= x->x_buf_loopstart[t_bufnum];
							buffer_index = loop_start + MODI(buffer_index,x->x_buf_looplength[t_bufnum]);
							
							val = t_bufsamples[(buffer_index * t_bufchan) + t_bchan_offset_mod];
			      			
			      			lindex += lincr;
			      			
							// calcul de la valeur env[i]*son[i]*amp[i]
							val = x->x_env[i]*val*x->Vamp[i]*(*fade_table++);

                            // calcul du pan en fonction de Vhp1[i] et Vhp2[i]
                            for(hp = 0; hp < x->x_nouts; hp++ )
                                outs[hp][j] += x->Vhp[i][hp]*val;
						}
					
				else // if non loop
				if( x->x_sinterp == 1 ) // non loop + interp 
					for(j= dsp_i_begin; j < dsp_i_end; j++)
					{
					
						// Lecture de l'enveloppe
						buffer_index = base_lindex_env + interp_get_int( lindex_env );
						interp_table_index = interp_get_table_index( lindex_env );
		      			x->x_env[i] = interp_table[interp_table_index].a * t_envsamples[buffer_index]
		      							+ interp_table[interp_table_index].b * t_envsamples[buffer_index + 1];
		      			
		      			lindex_env += lincr_env;
		      			
		      			//x->envind[i] += x->envinc[i]; // 2 REMOVE
						
						// Lecture de la forme d'onde
			    	  	buffer_index = base_lindex + interp_get_int( lindex );
						interp_table_index = interp_get_table_index( lindex );
		      			val = interp_table[interp_table_index].a * t_bufsamples[(buffer_index * t_bufchan) + t_bchan_offset_mod]
		      							+ interp_table[interp_table_index].b * t_bufsamples[((buffer_index + 1) * t_bufchan) + t_bchan_offset_mod];
		      			
		      			lindex += lincr;

						// calcul de la valeur env[i]*son[i]*amp[i]
						val = x->x_env[i]*val*x->Vamp[i]*(*fade_table++);

                        // calcul du pan en fonction de Vhp1[i] et Vhp2[i]
                        for(hp = 0; hp < x->x_nouts; hp++ )
                            outs[hp][j] += x->Vhp[i][hp]*val;
						
						
					}
                else if ( x->x_sinterp == 2 ) // non loop + interp SINC ///**************************************** ///
                    for(j= dsp_i_begin; j < dsp_i_end; j++)
                    {
                        
                        // Lecture de l'enveloppe
                        buffer_index = base_lindex_env + interp_get_int( lindex_env );
                        interp_table_index = interp_get_table_index( lindex_env );
                        x->x_env[i] = interp_table[interp_table_index].a * t_envsamples[buffer_index]
                        + interp_table[interp_table_index].b * t_envsamples[buffer_index + 1];
                        
                        lindex_env += lincr_env;
                        
                        val = 0.;
                        long temp_iind = (long) temp_sind ;
                        fract_index = temp_sind - (double)temp_iind;
                        long fract_index_i = (double) (fract_index * (double)sinc_phasei);
                        double highpass_fact = (finc > 1.)? 1/finc : 1. ;
                        long norm_index = 0;
                        
                        for(m=-7;m<9;m++)
                        {
                            k = temp_iind + m;
                            k = SAT(k,0,t_bufframes);
                            val += x->x_blackman_table[x_sinc_table_offset[m+7]+fract_index_i] * x->x_sinc_interp_table[(long)(8192+ highpass_fact * (8192 - (x_sinc_table_offset[m+7]+fract_index_i)))] * t_bufsamples[(k * t_bufchan) + t_bchan_offset_mod];
                         //   printf( " index %f ",x->x_sinc_interp_table[x_sinc_table_offset[m+7]]);
                        }
                        
                        
                        temp_sind += finc;
                        
                        norm_index = (long)(highpass_fact*SINC_TABLE_SIZE)-1;
                        //printf("nindex %d\n",norm_index);
                        
                        hp_norm = x->x_sinc_norm_table[norm_index];
                        
                        
                        // calcul de la valeur env[i]*son[i]*amp[i]
                        val = x->x_env[i]*val*x->Vamp[i]*hp_norm*(*fade_table++);
                        
                        // calcul du pan en fonction de Vhp1[i] et Vhp2[i]
                        for(hp = 0; hp < x->x_nouts; hp++ )
                            outs[hp][j] += x->Vhp[i][hp]*val;
                        
                        
                    }
				else		// non loop + non interp
					for(j= dsp_i_begin; j < dsp_i_end; j++)
					{
					
						// Lecture de l'enveloppe
						buffer_index = base_lindex_env + interp_get_int( lindex_env );
		      			x->x_env[i] = t_envsamples[buffer_index];
		      			
		      			lindex_env += lincr_env;
		      			
		      			//x->envind[i] += x->envinc[i]; // 2 REMOVE
						
						// Lecture de la forme d'onde
			    	  	buffer_index = base_lindex + interp_get_int( lindex );
		      			val = t_bufsamples[(buffer_index * t_bufchan) + t_bchan_offset_mod];
		      			
		      			lindex += lincr;

						// calcul de la valeur env[i]*son[i]*amp[i]
						val = x->x_env[i]*val*x->Vamp[i]*(*fade_table++);

                        // calcul du pan en fonction de Vhp1[i] et Vhp2[i]
                        for(hp = 0; hp < x->x_nouts; hp++ )
                            outs[hp][j] += x->Vhp[i][hp]*val;
						
						
					}
			
				//post("%d %d dsp_i_begin",dsp_i_begin,dsp_i_end);
				
				//********************************
				//  MAJ de l'Žtat des grains
				
				x->x_sind[i] = target_findex;
				
				x->x_ind[i] += nech_process_sat;
				
				if( ((x->x_remain_ind[i] -= nech_process_sat) <= 0) || x->x_voiceOn[i] ==3 )
				{	
					x->x_voiceOn[i] = 0;
					//post("remain kill");
				}
				
				// decremente delai
				x->x_delay[i] = MAX(x->x_delay[i] - N,0);
					
					

			}
            else {
				
				//sinon = si la voix est libre// 
	   			x->x_voiceOn[i] = 0;		
			}
			 
			next:
			j=0; //DUMMY
			   
		} // fin for
			
        		  
		x->x_sigin = sigin;

		return ;

	}

	zero:
		// Pas de declenchement de grains
        for(hp = 0; hp < x->x_nouts; hp++ )
            memset(outs[hp],0,N*sizeof(double));
	
    out:    
		return;
}
//***********************************************************************************//
