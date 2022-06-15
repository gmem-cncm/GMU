	
import com.cycling74.max.*;
import com.cycling74.msp.*;

/**
 * a collection of buffer utilities
 * 
 * created on Apr 12, 2004 
 * @author bbn
 * 
 * TODO support joint operations on more than one channel.  
 * ie, DC offset
 *
 */
public class buffer_edit extends MaxObject {
	
	String bufname = null;
	float[] ref_buf;
	float[] tmp_buf;
	long tmp_length = 4096;
	
	buffer_edit(Atom[] a) {
		
		declareInlets(new int[]{DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL,DataTypes.ALL});
		createInfoOutlet(false);
		if (a.length>0) 
			set(a[0].toString());
	}
	
	public void set(String s) {
		bufname = s;
		tmp_length = MSPBuffer.getFrames(s);
		
		tmp_buf = new float[(int)tmp_length];
		ref_buf = new float[(int)tmp_length];
		
	}
	
	
	public void merge(Atom[] a) {
		long maxFrames = 0;
		int channelSum = 0;
		int[] channels = new int[a.length];
		long[] frames = new long[a.length];
		String[] name = new String[a.length];
		for (int i=0;i<a.length;i++) {
			name[i] = a[i].toString();
			channels[i] = MSPBuffer.getChannels(name[i]);
			channelSum+=channels[i];
			frames[i] = MSPBuffer.getFrames(name[i]);
			if (frames[i] > maxFrames)
				maxFrames = frames[i]; 
		}
		MSPBuffer.setFrames(bufname, channelSum, maxFrames);
		int count = 1;
		for (int i=0;i<a.length;i++) {
			for (int j=0;j<channels[i];j++) {
				float[] data = MSPBuffer.peek(name[i], j+1);
				MSPBuffer.poke(bufname, count++, data);
			}
		}
		
		tmp_length = MSPBuffer.getFrames(bufname);
		
		tmp_buf = new float[(int)tmp_length]; 		
		ref_buf = new float[(int)tmp_length];
	}
	
	public void split(Atom[] a) {
		int channels = MSPBuffer.getChannels(bufname);
		if (a.length != channels) {
			error("buf.Op: need " +channels+ " buffer name arguments "
					+" to split "+bufname);
			return;
		}
		long chanSize = MSPBuffer.getSize(bufname)/channels;
		for (int i=0;i<channels;i++) {
			String name = a[i].toString();
			MSPBuffer.setSize(name, 1, chanSize);
			MSPBuffer.poke(name, MSPBuffer.peek(bufname, i+1));
		}
	}
	
	public void equals(String thatbuf) {
		boolean equal = true;
		long size = MSPBuffer.getSize(bufname);
		if (size != MSPBuffer.getSize(thatbuf))
			equal = false;
		else if (MSPBuffer.getChannels(bufname) 
					!= MSPBuffer.getChannels(thatbuf))
			equal = false;
		else {
			float[] thisData = MSPBuffer.peek(bufname);
			float[] thatData = MSPBuffer.peek(thatbuf);
			int index = 0;
			while ((equal)&&(index < size)) {
				if (thisData[index] != thatData[index])
					equal = false;
				index++;
			}
			outlet(0, equal);
		}
	}
	
	public void copyFrom(String frombuf) {
		long size = MSPBuffer.getSize(frombuf);
		int channels = MSPBuffer.getChannels(frombuf);
		MSPBuffer.setSize(bufname, channels, size);
		MSPBuffer.poke(bufname, MSPBuffer.peek(frombuf));
		
		tmp_length = MSPBuffer.getFrames(bufname);

		tmp_buf = new float[(int)tmp_length]; 		
		ref_buf = new float[(int)tmp_length];
	}
	
	public void copyInto(String destbuf) {
		long size = MSPBuffer.getSize(bufname);
		int channels = MSPBuffer.getChannels(bufname);
		MSPBuffer.setSize(destbuf, channels, size);
		MSPBuffer.poke(destbuf, MSPBuffer.peek(bufname));
	}
	
	public void max(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		float max = Float.MIN_VALUE;
		int index = 0;
		for (int i=0;i<samps.length;i++) 
			if (samps[i] > max) {
				max = samps[i];
				index = i;
			}
		outlet(0, new Atom[] {Atom.newAtom(index), Atom.newAtom(max)});
	}

	public void min(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		float min = Float.MAX_VALUE;
		int index = 0;
		for (int i=0;i<samps.length;i++) 
			if (samps[i] < min) {
				min = samps[i];
				index = i;
			}
		outlet(0, new Atom[] {Atom.newAtom(index), Atom.newAtom(min)});
	}

	public void removeDC(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		double total = 0;
		for (int i=0;i<samps.length;i++) {
			total += samps[i];
		}
		float shift = (float)(total/(double)samps.length);
		for (int i=0;i<samps.length;i++) 
			samps[i] -= shift;
		MSPBuffer.poke(bufname, channel, samps);
	}
	
	public void normalize(int channel, float f) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		double max = Double.MIN_VALUE;
		for (int i=0;i<samps.length;i++) 
			if (Math.abs(samps[i]) > max)
				max = samps[i];
		double scale = ((double)Math.abs(f))/max;
		for (int i=0;i<samps.length;i++) 
			samps[i] *= scale;
		MSPBuffer.poke(bufname, channel, samps);
	}
	
	public void multiply(int channel, float f) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		for (int i=0;i<samps.length;i++) 
			samps[i] *= f;
		MSPBuffer.poke(bufname, channel, samps);
	}
	
	public void gain(float f) {
		float factor = (float)Math.pow(10.0, f/20.);
		int chans = MSPBuffer.getChannels(bufname);
		for (int i=1;i<=chans;i++) {
			multiply(i, factor);
		}
	}
	
	public void add(int channel, float f) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		for (int i=0;i<samps.length;i++) 
			samps[i] += f;
		MSPBuffer.poke(bufname, channel, samps);
	}
	
	public void reverse(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		float[] temp = new float[samps.length];
		for (int i=0;i<samps.length;i++) 
			temp[i] = samps[samps.length-i-1];
		MSPBuffer.poke(bufname, channel, temp);
	}
	
	public void peek(int chan, long index) {
		outlet(0, MSPBuffer.peek(bufname, chan, index));
	}
	
	public void poke(int chan, long index, float f) {
		MSPBuffer.poke(bufname, chan, index, f);
	}
	
	public void pokeAll(int chan, float f) {
		float[] data = new float[(int)MSPBuffer.getSize(bufname)];
		for (int i=0;i<data.length;i++) {
			data[i] = f;
		}
		MSPBuffer.poke(bufname, chan, data);
	}
	
	public void sum(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		double total = 0;
		for (int i=0;i<samps.length;i++) {
			total += samps[i];
		}
		outlet(0, total);
	}
	
	public void stddev(int channel) {
		outlet(0, Math.sqrt(doVariance(channel)));
	}  
	
	public void variance(int channel) {
		outlet(0, doVariance(channel));
	}
	
	private double doVariance(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		double total = 0;
		for (int i=0;i<samps.length;i++) {
			total += samps[i];
		}
		double mean = total / (double)samps.length;
		total = 0;
		for (int i=0;i<samps.length;i++) {
			total += Math.pow(samps[i]-mean,2.);
		}
		return total/(double)(samps.length - 1);
	}
	
	public void average(int channel) {
		float[] samps = MSPBuffer.peek(bufname, channel);
		double total = 0;
		for (int i=0;i<samps.length;i++) {
			total += samps[i];
		}
		outlet(0, total/(double)samps.length);
	}
	
	public void mean(int channel) {
		average(channel);
	}
	
	public void getLength() {
		outlet(0, MSPBuffer.getLength(bufname));
	}
		
	public void setLength(int i, double d) {
		MSPBuffer.setLength(bufname, i, d);
		tmp_length = MSPBuffer.getFrames(bufname);
		tmp_buf = new float[(int)tmp_length]; 		
		ref_buf = new float[(int)tmp_length];
	}
	
	public void getSize() {
		outlet(0, MSPBuffer.getSize(bufname));
	}
	
	public void setSize(int channels, long size) {
		MSPBuffer.setSize(bufname, channels, size);
		
		tmp_length = MSPBuffer.getFrames(bufname);
		tmp_buf = new float[(int)tmp_length]; 		
		ref_buf = new float[(int)tmp_length];
	}
	
	public void getChannels() {
		outlet(0, MSPBuffer.getChannels(bufname));
	}
	
	/*
	 * My extension
	 */
	
	/*
	 *  TODO : Selection mode
	 */
	
	/*
	 * Generator - effect functions
	 */
	
	/*
	 * WiNDOWSSSSSS
	 */
	private void hanning(float[] out_tab)
	{
		int outlength = out_tab.length;
		double twoPIon = 2*Math.PI/(outlength-1);
		
		// w[i] = 0.5*(1-cos(2*Pi*i/(n-1))
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)(0.5 * (1-Math.cos(i*twoPIon)));
		}
	}
	
	private void hamming(float[] out_tab)
	{
		int outlength = out_tab.length;
		double twoPIon = 2*Math.PI/(outlength-1);
		
		// w[i] = 0.54-0.46*cos(2*Pi*i/(n-1))
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)(0.54 -0.46*Math.cos(i*twoPIon));
		}
	}
	
	private void blackman(float[] out_tab)
	{
		int outlength = out_tab.length;
		double twoPIon = 2*Math.PI/(outlength-1);
		double fourPIon = 4*Math.PI/(outlength-1);
		
		// w[n] := 0.42323 - 0.49755*cos(2*Pi*n/(n-1)) + 0.07922*cos(4*Pi*n/(n-1))
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)(0.42323 - 0.49755*Math.cos(twoPIon*i) + 0.07922*Math.cos(fourPIon*i));
		}
	}
	
	private void gauss(Atom[] a , float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		int outlength2 = (int)(outlength/2);
		double sigma = 0.02;
		double den;
		
		if(argl > 0 && a[0].isFloat())
			sigma = a[0].getFloat() / 100.;
		
		den = Math.pow((double)outlength*sigma,2);
		// exp(-0.5*pow((x-(N/2))/(N*sigma),2))
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)(Math.exp(-0.5*Math.pow((i-outlength2)/den,2)));
		}
	}
	
	private void expodecay(Atom[] a , float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		double sigma = 20.;
		double den;
		
		if(argl > 0 && a[0].isFloat())
			sigma = a[0].getFloat();
		
		den = (double)outlength/sigma;
		// exp(-0.5*pow((x-(N/2))/(N*sigma),2))
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)(Math.exp(-(float)i/den));
		}
	}
	/*
	 * Functions
	 */
	private void constant(Atom[] a , float[] out_tab)
	{
		int argl = a.length;
		float value = 0.f;
		int outlength = out_tab.length;
		
		if(argl>0 && a[0].isFloat())
			value = a[0].getFloat();
		
		for(int i=0; i<outlength; i++)
			out_tab[i] = value;
	}
	
	private void sin(Atom[] a , float[] out_tab)
	{
		int argl = a.length;
		float freq = 1.f;
		double phase_incr,phase = 0.;
		int outlength = out_tab.length;
		
		if(argl>0 && a[0].isFloat())
			freq = a[0].getFloat();
		if(argl>1 && a[1].isFloat())
			phase = (double) a[1].getFloat();
		
		phase *= 2 * Math.PI;
		phase_incr = 2*Math.PI*freq/outlength;
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)Math.sin(phase);
			phase+=phase_incr;
		}
	}
	
	private void cos(Atom[] a , float[] out_tab)
	{
		int argl = a.length;
		float freq = 1.f;
		double phase_incr,phase = 0.;
		int outlength = out_tab.length;
		
		if(argl>0 && a[0].isFloat())
			freq = a[0].getFloat();
		if(argl>1 && a[1].isFloat())
			phase = (double) a[1].getFloat();
		
		phase *= 2 * Math.PI;
		phase_incr = 2*Math.PI*freq/outlength;
		
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)Math.cos(phase);
			phase+=phase_incr;
		}
	}
	
	private void rand(Atom[] a, float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		float freq = (float)outlength;
		float amp = 0.5f;
		float mean = 0.5f;
		int period,j=0;
		float rand_value;
		
		if(argl>0 && a[0].isFloat())
			freq = a[0].getFloat();
		if(argl>1 && a[1].isFloat())
			amp = a[1].getFloat();
		if(argl>2 && a[2].isFloat())
			mean = a[2].getFloat();
		
		period = (int)Math.max(((float)outlength/freq),1.);
		
		rand_value = (float)(mean+amp*2*(Math.random()-0.5));
		for(int i=0; i<outlength; i++)
		{
			out_tab[i] = (float)rand_value;
			if(++j==period)
			{
				rand_value = (float)(mean+amp*2*(Math.random()-0.5));
				j=0;
			}
			
		}
		
		
	}
	
	private void repeat(Atom[] a, float[] buffer, float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		double freq_start = 2.f;
		double freq_end;
		float freq_exp = 1.f;
		double freq_diff;
		double where;
		double maxpos = (double) (outlength-1);
		float[] indexes = new float[outlength];
		
		if(argl>0 && a[0].isFloat())
			freq_start = a[0].getFloat();
		freq_end = freq_start;
		if(argl>1 && a[1].isFloat())
			freq_end = a[1].getFloat();
		if(argl>2 && a[2].isFloat())
			freq_exp = a[2].getFloat();
		
		//freq = freq_start;
		freq_diff = freq_end - freq_start;
		//freq_incr = (float)((freq_end - freq_start)/maxpos);
		
		for(int i=0; i<outlength;i++)
		{
			where = (double)i/outlength;
			indexes[i] = (float)((((freq_start + freq_diff * Math.pow(where,freq_exp))*where)%1.f)*maxpos);
			
			//phase = (float)((phase + freq) % maxpos); 
			//freq += freq_incr;
		}
		
		interpolate(buffer,out_tab,indexes);
		
	}
	
	private void bpf(Atom[] a, float[] out_tab)
	{
		int argl = a.length;
		int numpoints=0;
		float time,oldtime=-0.1f;
		int[] times = new int[256];
		float[] values = new float[256];
		int argind = 0;
		
		int outlength = out_tab.length;
		int seglength;
		float segslope;
		float curval;
		
		//  get the arguments <value> <time> <value> ....
		if(argl<2)
		{
			error("buffer_edit : bpf needs at list one point");
			return;
		}
		argl -= 2;
		
		while(argl>=0)
		{
			values[numpoints]=a[argind++].getFloat();
			time = a[argind++].getFloat();
			times[numpoints++]=(int) (time * outlength);
			if(time < 0. || time > 1.)
			{
				error("buffer_edit : bpf time must be between 0. & 1.");
				return;
			}
			
			if(time<oldtime)
			{
				error("buffer_edit : bpf time must be in ascending order");
				return;
			}
			oldtime = time;
			argl -= 2;
		}
		
		// generate the interpolated values
		for(int j=0; j<numpoints-1; j++)
		{
			seglength = times[j+1] - times[j];
			segslope = (values[j+1] - values[j])/seglength;
			curval = values[j];
			for(int i=times[j]; i<times[j+1]; i++)
			{
				//post("curval "+curval+" times "+ i);
				out_tab[i] = curval;
				//post("curval "+curval+" times "+ i);
				curval += segslope;
			}
			
		}
		//padding if last point is not at the end of the buffer
		if(times[numpoints-1] < outlength)
		{
			curval = values[numpoints-1];
			for(int i=times[numpoints-1]; i<outlength-1;i++)
			{
				//post("curval "+curval+" times "+ i);
				out_tab[i] = curval;
			}
				
		}
		
		
	}

	// TODO : BPF "before" padding
	
	public void mouseedit(Atom[] a)
	{
		int argl = a.length;
		int numpoints=0;
		float time;
		int[] times = new int[256];
		float[] values = new float[256];
		int argind = 0;
		
		int outlength = tmp_buf.length;
		int seglength;
		float segslope;
		float curval;
		
		//  get the arguments <value> <time> <value> ....
		if(argl<2)
		{
			error("buffer_edit : mouseedit needs at list one point");
			return;
		}
		argl -= 2;
		
		while(argl>=0)
		{
			time = a[argind++].getFloat();
			values[numpoints]=a[argind++].getFloat();
			times[numpoints++]=(int) (time * outlength);
			if(time < 0. || time > 1.)
			{
				error("buffer_edit : mouseedit time must be between 0. & 1.");
				return;
			}
			
			argl -= 2;
		}
		
		// peek from main buffer
		copy_buf(MSPBuffer.peek(bufname),tmp_buf);
		
		//		 generate the interpolated values
		for(int j=0; j<numpoints-1; j++)
		{
			seglength = times[j+1] - times[j];
			if(seglength > 0.)
			{
				segslope = (values[j+1] - values[j])/seglength;
				curval = values[j];
				for(int i=times[j]; i<=times[j+1]; i++)
				{
					tmp_buf[i] = curval;
					curval += segslope;
				}
			}else if(seglength < 0.)
			{
				seglength = - seglength;
				segslope = (values[j+1] - values[j])/seglength;
				curval = values[j];
				for(int i=times[j]; i>=times[j+1]; i--)
				{
					tmp_buf[i] = curval;
					curval += segslope;
				}
			}else
				tmp_buf[times[j+1]] = values[j+1];
			
		}
		
		// poke to main buffer
		MSPBuffer.poke(bufname,tmp_buf);
		
	}
	
	/*
	 * 
	 * APPLY METHODS
	 */
	
	
	// method to store current buffer temporarly for edition like mult, add, crossfade, ...
	public void pushBuffer()
	{
		copy_buf(MSPBuffer.peek(bufname),ref_buf);
	}
	
	public void popBuffer()
	{
		MSPBuffer.poke(bufname,ref_buf);
	}
	
	private void op(Atom[] a,float[] out_buf)
	{
		String opname;
		
			opname = a[0].getString();

			
			if(opname.compareTo("hanning") == 0)
				hanning(out_buf);
			else
			if(opname.compareTo("hamming") == 0)
				hamming(out_buf);
			else
			if(opname.compareTo("blackman") == 0)
				blackman(out_buf);
			else
			if(opname.compareTo("gauss") == 0)
				gauss(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("expodecay") == 0)
				expodecay(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("constant") == 0)
				constant(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("sin") == 0)
				sin(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("cos") == 0)
				cos(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("rand") == 0)
				rand(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("bpf") == 0)
				bpf(Atom.removeFirst(a),out_buf);
			else
			if(opname.compareTo("repeat") == 0)
				repeat(Atom.removeFirst(a),ref_buf,out_buf);
			else
			if(opname.compareTo("smooth") == 0)
				smooth(Atom.removeFirst(a),ref_buf,out_buf);
			else
			if(opname.compareTo("pow") == 0)
				pow(Atom.removeFirst(a),ref_buf,out_buf);
			else
			if(opname.compareTo("translate") == 0)
				translate(Atom.removeFirst(a),ref_buf,out_buf);
			else
			if(opname.compareTo("rotate") == 0)
				rotate(Atom.removeFirst(a),ref_buf,out_buf);
			else
			if(opname.compareTo("flipY") == 0)
				flipY(Atom.removeFirst(a),ref_buf,out_buf);
			else
				error("buffer_edit : undefined function "+opname);

		
	}

	public void replace(Atom[] a)
	{
		int argl = a.length;
		
		if(argl == 0)
		{
			post("replace need a function/generator");
			return;
		}
		if(a[0].isString())
		{
			op(a,tmp_buf);

			MSPBuffer.poke(bufname,tmp_buf);
		}else
		{
			post("replace expect a function/generator");
		}
		
		outlet_notify();
	}

	public void mult(Atom[] a)
	{
		int argl = a.length;
		
		if(argl == 0)
		{
			post("mult need a function/generator");
			return;
		}	
		if(a[0].isString())
		{
			op(a,tmp_buf);
			mult_buf(ref_buf, tmp_buf, tmp_buf);
			MSPBuffer.poke(bufname,tmp_buf);
		}else
		{
			post("mult expect a function/generator");
		}
			
		outlet_notify();
	}

	
	public void add(Atom[] a)
	{
		int argl = a.length;
		
		if(argl == 0)
		{
			post("add need a function/generator");
			return;
		}
		if(a[0].isString())
		{
			op(a,tmp_buf);
			add_buf(ref_buf, tmp_buf, tmp_buf);
			MSPBuffer.poke(bufname,tmp_buf);
		}else
		{
			post("add expect a function/generator");
		}
			
		outlet_notify();
	}
	
	
	public void xfade(Atom[] a)
	{
		int argl = a.length;
		float factor;
		
		if(argl < 2)
		{
			post("xfade needs a factor and a function/generator");
			return;
		}
		if(!a[0].isFloat())
		{
			post("xfade factor must be float 0.-1.");
			return;
		}
		
		factor = a[0].getFloat();
		
		if(a[1].isString())
		{
			op(Atom.removeFirst(a),tmp_buf);
			xfade_buf(factor,ref_buf, tmp_buf, tmp_buf);
			MSPBuffer.poke(bufname,tmp_buf);
		}else
		{
			post("xfade expect a function/generator");
		}
			
		outlet_notify();
	}
	
	
	/*
	 * Edition-Effects
	 */
	
	private void smooth(Atom[] a, float[] buffer, float[] out_tab)
	{
		int argl = a.length;
		double factor=1.;
		if(argl>0 && a[0].isFloat())
			factor = Math.max(a[0].getFloat(),0.);
		int factor_ceil = (int)Math.ceil(factor);
		int kernel_length = 2*factor_ceil+1;
		float[] kernel = new float[kernel_length];
		float sum=1.f;
		
		//hanning kernel
		for(int i=1; i< factor_ceil; i++)
		{
			sum+=2*(kernel[factor_ceil-i]=(float)(0.5*(1+Math.cos(i*Math.PI/factor))));
			
			//post(" mlk"+kernel[i]);
		}
		
		for(int i=1; i< factor_ceil; i++)
			kernel[factor_ceil+i]=(kernel[factor_ceil-i]/=sum);
		
		kernel[factor_ceil]=1.f/sum;
		// do filter
		filter(buffer,out_tab,kernel);
	}
	
	private void pow(Atom[] a, float[] buffer, float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		double exponent=2.;
		float in;
		float sign;
		
		if(argl>0 && a[0].isFloat())
			exponent = Math.max(a[0].getFloat(),0.);
		
		for(int i=0; i<outlength; i++)
		{
			in = buffer[i];
			sign = ((in>0.) ? 1.f : -1.f );
			out_tab[i]=sign*(float)Math.pow(Math.abs(in),exponent);
		}
		
	}

	private void translate(Atom[] a, float[] buffer, float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		int trans=0, in_index;
		if(argl>0 )
			if(a[0].isFloat())
				trans = (int)(Math.max(Math.min(a[0].getFloat(),1.),-1.)*outlength);
			else if(a[0].isInt())
				trans = Math.max(Math.min(a[0].getInt(),outlength),-outlength);
		
		for(int i=0; i< outlength; i++)
		{
			in_index = i-trans;
			if(in_index >= 0 && in_index < outlength)
				out_tab[i]=buffer[in_index];
			else
				out_tab[i]=0.f;
		}
	}
		
	private void rotate(Atom[] a, float[] buffer, float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		int trans=0, in_index;
		if(argl>0 )
			if(a[0].isFloat())
				trans = (int)(Math.max(Math.min(a[0].getFloat(),1.),-1.)*outlength);
			else if(a[0].isInt())
				trans = Math.max(Math.min(a[0].getInt(),outlength),-outlength);
		
		for(int i=0; i< outlength; i++)
		{
			in_index = (outlength+i-trans) % outlength ;
			out_tab[i]=buffer[in_index];

		}
	}
	
	private void flipY(Atom[] a, float[] buffer, float[] out_tab)
	{
		int argl = a.length;
		int outlength = out_tab.length;
		float center = 0.5f;
		int icenter, in_index;
		
		clear_buf(out_tab);
		
		if(argl>0 && a[0].isFloat() )
			center = Math.max(Math.min(a[0].getFloat(),1.f),0.f);
		icenter = (int)(center * outlength);
		
		for(int i=0; i< outlength; i++)
		{
			in_index = 2*icenter - i ;
			if(in_index>=0 && in_index<outlength)
				out_tab[i]=buffer[in_index];
		}
	}
	
	
	/*
	 * Utility function
	 */
	
	private void outlet_notify()
	{
		outlet(1,Atom.newAtom("bang"));
	}
	
	private void clear_buf(float[] buf)
	{
		int length = buf.length;
		
		for(int i=0; i< length; i++)
			buf[i] = 0.f;
	}
	
	private void interpolate(float[] in, float[] out, float[] indexes)
	{
		int outlength = out.length;
		int index;
		float ifact;
		
		for(int i=0; i<outlength; i++)
		{
			index = (int)indexes[i];
			ifact = indexes[i] - index;
			out[i] = (1-ifact)*in[index] + ifact*in[index+1];
			
		}
	}

	private void filter(float[] in, float[] out, float[] kernel)
	{
		int kernel_length = kernel.length;
		int out_length = out.length;
		int kernel_offset = (int)Math.ceil((double)kernel_length/2);
		int kernel_left = Math.max(kernel_offset-1,0);
		int kernel_right = kernel_length-kernel_offset;
		int k;
		
		for(int i=0; i<out_length ; i++)
		{
			out[i]=0.f;
			k=0;
			for(int j=-kernel_left; j<kernel_right;j++,k++)
			{
				out[i]+= kernel[k]*in[Math.min(Math.max(i+j,0),out_length-1)];
			}
		}
		
	}
	
	private void copy_buf(float[] in, float[] out)
	{
		int length1 = in.length;
		int length2 = out.length;
		int length = Math.min(length1,length2);
		
		for(int i=0; i< length; i++)
			out[i] = in[i];
		
	}
	
	private void mult_buf(float[] buf1, float[] buf2, float[] out_buf)
	{
		int length1 = buf1.length;
		int length2 = buf2.length;
		int length = Math.min(length1,length2);
		
		for(int i=0; i< length; i++)
			out_buf[i] = buf1[i] * buf2[i];
	}
	
	private void add_buf(float[] buf1, float[] buf2, float[] out_buf)
	{
		int length1 = buf1.length;
		int length2 = buf2.length;
		int length = Math.min(length1,length2);
		
		for(int i=0; i< length; i++)
			out_buf[i] = buf1[i] + buf2[i];
	}
	
	private void xfade_buf(float factor, float[] buf1, float[] buf2, float[] out_buf)
	{
		int length1 = buf1.length;
		int length2 = buf2.length;
		int length = Math.min(length1,length2);
		factor = Math.max(Math.min(factor,1.f),0.f);
		
		for(int i=0; i< length; i++)
			out_buf[i] = (1-factor)*buf1[i] + factor*buf2[i];
	}
	
}
