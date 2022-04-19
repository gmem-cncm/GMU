
/*
    simple javascript list statistics object
    reports min mean max sum product of elements in a list
*/

// set up inlets/outlets/assist strings
outlets = 1;

function anything()
{

    len = arguments.length;

    for (i=0;i<len;i++) 
    {
        v = arguments[i];
       post(v);

    }    
    outlet(0,arguments);        
}
