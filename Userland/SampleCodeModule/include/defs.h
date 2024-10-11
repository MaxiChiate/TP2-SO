#ifndef _DEFS_H
#define _DEFS_H

    // https://www.onlinegdb.com/online_c_debugger
// Script util para armar estas matrices: (ejemplo @capitals)

    // #include <stdio.h>

    // int main()
    // {
    //     printf("{");
        
    //     for(int i=0; i<257; ++i)    {
    //         printf(" /*");
    //         if(i<100)
    //             printf("0");
    //         if(i<10)
    //             printf("0");
    //         printf("%d*/%d, ",i, i >= 'A' && i <= 'Z');
    //     }
        
    //     printf("}");

    //     return 0;
    // }

    

    unsigned char digits[257]= { /*000*/0, 
        /*001*/0, /*002*/0, /*003*/0, /*004*/0, /*005*/0, /*006*/0, /*007*/0, /*008*/0, 
        /*009*/0, /*010*/0, /*011*/0, /*012*/0, /*013*/0, /*014*/0, /*015*/0, /*016*/0, 
        /*017*/0, /*018*/0, /*019*/0, /*020*/0, /*021*/0, /*022*/0, /*023*/0, /*024*/0, 
        /*025*/0, /*026*/0, /*027*/0, /*028*/0, /*029*/0, /*030*/0, /*031*/0, /*032*/0, 
        /*033*/0, /*034*/0, /*035*/0, /*036*/0, /*037*/0, /*038*/0, /*039*/0, /*040*/0, 
        /*041*/0, /*042*/0, /*043*/0, /*044*/0, /*045*/0, /*046*/0, /*047*/0, /*048*/1, 
        /*049*/1, /*050*/1, /*051*/1, /*052*/1, /*053*/1, /*054*/1, /*055*/1, /*056*/1, 
        /*057*/1, /*058*/0, /*059*/0, /*060*/0, /*061*/0, /*062*/0, /*063*/0, /*064*/0, 
        /*065*/0, /*066*/0, /*067*/0, /*068*/0, /*069*/0, /*070*/0, /*071*/0, /*072*/0, 
        /*073*/0, /*074*/0, /*075*/0, /*076*/0, /*077*/0, /*078*/0, /*079*/0, /*080*/0, 
        /*081*/0, /*082*/0, /*083*/0, /*084*/0, /*085*/0, /*086*/0, /*087*/0, /*088*/0, 
        /*089*/0, /*090*/0, /*091*/0, /*092*/0, /*093*/0, /*094*/0, /*095*/0, /*096*/0, 
        /*097*/0, /*098*/0, /*099*/0, /*100*/0, /*101*/0, /*102*/0, /*103*/0, /*104*/0, 
        /*105*/0, /*106*/0, /*107*/0, /*108*/0, /*109*/0, /*110*/0, /*111*/0, /*112*/0, 
        /*113*/0, /*114*/0, /*115*/0, /*116*/0, /*117*/0, /*118*/0, /*119*/0, /*120*/0, 
        /*121*/0, /*122*/0, /*123*/0, /*124*/0, /*125*/0, /*126*/0, /*127*/0, /*128*/0, 
        /*129*/0, /*130*/0, /*131*/0, /*132*/0, /*133*/0, /*134*/0, /*135*/0, /*136*/0, 
        /*137*/0, /*138*/0, /*139*/0, /*140*/0, /*141*/0, /*142*/0, /*143*/0, /*144*/0, 
        /*145*/0, /*146*/0, /*147*/0, /*148*/0, /*149*/0, /*150*/0, /*151*/0, /*152*/0, 
        /*153*/0, /*154*/0, /*155*/0, /*156*/0, /*157*/0, /*158*/0, /*159*/0, /*160*/0, 
        /*161*/0, /*162*/0, /*163*/0, /*164*/0, /*165*/0, /*166*/0, /*167*/0, /*168*/0, 
        /*169*/0, /*170*/0, /*171*/0, /*172*/0, /*173*/0, /*174*/0, /*175*/0, /*176*/0, 
        /*177*/0, /*178*/0, /*179*/0, /*180*/0, /*181*/0, /*182*/0, /*183*/0, /*184*/0, 
        /*185*/0, /*186*/0, /*187*/0, /*188*/0, /*189*/0, /*190*/0, /*191*/0, /*192*/0, 
        /*193*/0, /*194*/0, /*195*/0, /*196*/0, /*197*/0, /*198*/0, /*199*/0, /*200*/0, 
        /*201*/0, /*202*/0, /*203*/0, /*204*/0, /*205*/0, /*206*/0, /*207*/0, /*208*/0, 
        /*209*/0, /*210*/0, /*211*/0, /*212*/0, /*213*/0, /*214*/0, /*215*/0, /*216*/0, 
        /*217*/0, /*218*/0, /*219*/0, /*220*/0, /*221*/0, /*222*/0, /*223*/0, /*224*/0, 
        /*225*/0, /*226*/0, /*227*/0, /*228*/0, /*229*/0, /*230*/0, /*231*/0, /*232*/0, 
        /*233*/0, /*234*/0, /*235*/0, /*236*/0, /*237*/0, /*238*/0, /*239*/0, /*240*/0, 
        /*241*/0, /*242*/0, /*243*/0, /*244*/0, /*245*/0, /*246*/0, /*247*/0, /*248*/0, 
        /*249*/0, /*250*/0, /*251*/0, /*252*/0, /*253*/0, /*254*/0, /*255*/0, /*256*/0
        };

    unsigned char capitals[257]= { /*000*/0,  
    /*001*/0,  /*002*/0,  /*003*/0,  /*004*/0,  /*005*/0,  /*006*/0,  /*007*/0,  /*008*/0,  
    /*009*/0,  /*010*/0,  /*011*/0,  /*012*/0,  /*013*/0,  /*014*/0,  /*015*/0,  /*016*/0,  
    /*017*/0,  /*018*/0,  /*019*/0,  /*020*/0,  /*021*/0,  /*022*/0,  /*023*/0,  /*024*/0,  
    /*025*/0,  /*026*/0,  /*027*/0,  /*028*/0,  /*029*/0,  /*030*/0,  /*031*/0,  /*032*/0,  
    /*033*/0,  /*034*/0,  /*035*/0,  /*036*/0,  /*037*/0,  /*038*/0,  /*039*/0,  /*040*/0,  
    /*041*/0,  /*042*/0,  /*043*/0,  /*044*/0,  /*045*/0,  /*046*/0,  /*047*/0,  /*048*/0,  
    /*049*/0,  /*050*/0,  /*051*/0,  /*052*/0,  /*053*/0,  /*054*/0,  /*055*/0,  /*056*/0,  
    /*057*/0,  /*058*/0,  /*059*/0,  /*060*/0,  /*061*/0,  /*062*/0,  /*063*/0,  /*064*/0,  
    /*065*/1,  /*066*/1,  /*067*/1,  /*068*/1,  /*069*/1,  /*070*/1,  /*071*/1,  /*072*/1,  
    /*073*/1,  /*074*/1,  /*075*/1,  /*076*/1,  /*077*/1,  /*078*/1,  /*079*/1,  /*080*/1,  
    /*081*/1,  /*082*/1,  /*083*/1,  /*084*/1,  /*085*/1,  /*086*/1,  /*087*/1,  /*088*/1,  
    /*089*/1,  /*090*/1,  /*091*/0,  /*092*/0,  /*093*/0,  /*094*/0,  /*095*/0,  /*096*/0,  
    /*097*/0,  /*098*/0,  /*099*/0,  /*100*/0,  /*101*/0,  /*102*/0,  /*103*/0,  /*104*/0,  
    /*105*/0,  /*106*/0,  /*107*/0,  /*108*/0,  /*109*/0,  /*110*/0,  /*111*/0,  /*112*/0,  
    /*113*/0,  /*114*/0,  /*115*/0,  /*116*/0,  /*117*/0,  /*118*/0,  /*119*/0,  /*120*/0,  
    /*121*/0,  /*122*/0,  /*123*/0,  /*124*/0,  /*125*/0,  /*126*/0,  /*127*/0,  /*128*/0,  
    /*129*/0,  /*130*/0,  /*131*/0,  /*132*/0,  /*133*/0,  /*134*/0,  /*135*/0,  /*136*/0,  
    /*137*/0,  /*138*/0,  /*139*/0,  /*140*/0,  /*141*/0,  /*142*/0,  /*143*/0,  /*144*/0,  
    /*145*/0,  /*146*/0,  /*147*/0,  /*148*/0,  /*149*/0,  /*150*/0,  /*151*/0,  /*152*/0,  
    /*153*/0,  /*154*/0,  /*155*/0,  /*156*/0,  /*157*/0,  /*158*/0,  /*159*/0,  /*160*/0,  
    /*161*/0,  /*162*/0,  /*163*/0,  /*164*/0,  /*165*/0,  /*166*/0,  /*167*/0,  /*168*/0,  
    /*169*/0,  /*170*/0,  /*171*/0,  /*172*/0,  /*173*/0,  /*174*/0,  /*175*/0,  /*176*/0,  
    /*177*/0,  /*178*/0,  /*179*/0,  /*180*/0,  /*181*/0,  /*182*/0,  /*183*/0,  /*184*/0,  
    /*185*/0,  /*186*/0,  /*187*/0,  /*188*/0,  /*189*/0,  /*190*/0,  /*191*/0,  /*192*/0,  
    /*193*/0,  /*194*/0,  /*195*/0,  /*196*/0,  /*197*/0,  /*198*/0,  /*199*/0,  /*200*/0,  
    /*201*/0,  /*202*/0,  /*203*/0,  /*204*/0,  /*205*/0,  /*206*/0,  /*207*/0,  /*208*/0,  
    /*209*/0,  /*210*/0,  /*211*/0,  /*212*/0,  /*213*/0,  /*214*/0,  /*215*/0,  /*216*/0,  
    /*217*/0,  /*218*/0,  /*219*/0,  /*220*/0,  /*221*/0,  /*222*/0,  /*223*/0,  /*224*/0,  
    /*225*/0,  /*226*/0,  /*227*/0,  /*228*/0,  /*229*/0,  /*230*/0,  /*231*/0,  /*232*/0,  
    /*233*/0,  /*234*/0,  /*235*/0,  /*236*/0,  /*237*/0,  /*238*/0,  /*239*/0,  /*240*/0,  
    /*241*/0,  /*242*/0,  /*243*/0,  /*244*/0,  /*245*/0,  /*246*/0,  /*247*/0,  /*248*/0,  
    /*249*/0,  /*250*/0,  /*251*/0,  /*252*/0,  /*253*/0,  /*254*/0,  /*255*/0,  /*256*/0
    };

#endif
