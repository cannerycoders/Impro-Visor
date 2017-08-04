/*
 *  /MathJax/jax/output/HTML-CSS/autoload/mmultiscripts.js
 *  
 *  Copyright (c) 2010 Design Science, Inc.
 *
 *  Part of the MathJax library.
 *  See http://www.mathjax.org for details.
 * 
 *  Licensed under the Apache License, Version 2.0;
 *  you may not use this file except in compliance with the License.
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 */

MathJax.Hub.Register.StartupHook("HTML-CSS Jax Ready",function(){var c="1.1";var a=MathJax.ElementJax.mml,b=MathJax.OutputJax["HTML-CSS"];a.mmultiscripts.Augment({toHTML:function(H,F,A){H=this.HTMLcreateSpan(H);var M=this.HTMLgetScale();var k=b.createStack(H),f;var j=b.createBox(k);this.HTMLmeasureChild(this.base,j);if(this.data[this.base]){if(A!=null){b.Remeasured(this.data[this.base].HTMLstretchV(j,F,A),j)}else{if(F!=null){b.Remeasured(this.data[this.base].HTMLstretchH(j,F),j)}}}var K=b.TeX.x_height*M,z=b.TeX.scriptspace*M*0.75;var y=this.HTMLgetScripts(k,z);var l=y[0],e=y[1],o=y[2],i=y[3];var g=(this.data[1]||this).HTMLgetScale();var C=b.TeX.sup_drop*g,B=b.TeX.sub_drop*g;var w=j.bbox.h-C,n=j.bbox.d+B,L=0,E;if(j.bbox.ic){L=j.bbox.ic}if(this.data[this.base]&&(this.data[this.base].type==="mi"||this.data[this.base].type==="mo")){if(this.data[this.base].data.join("").length===1&&j.bbox.scale===1&&!this.data[this.base].Get("largeop")){w=n=0}}var G=this.getValues("subscriptshift","superscriptshift");G.subscriptshift=(G.subscriptshift===""?0:b.length2em(G.subscriptshift));G.superscriptshift=(G.superscriptshift===""?0:b.length2em(G.superscriptshift));var m=0;if(o){m=o.bbox.w+L}else{if(i){m=i.bbox.w-L}}if(m<0){m=0}b.placeBox(j,m,0);if(!e&&!i){n=Math.max(n,b.TeX.sub1*M,G.subscriptshift);if(l){n=Math.max(n,l.bbox.h-(4/5)*K)}if(o){n=Math.max(n,o.bbox.h-(4/5)*K)}if(l){b.placeBox(l,m+j.bbox.w+z-L,-n)}if(o){b.placeBox(o,0,-n)}}else{if(!l&&!o){f=this.getValues("displaystyle","texprimestyle");E=b.TeX[(f.displaystyle?"sup1":(f.texprimestyle?"sup3":"sup2"))];w=Math.max(w,E*M,G.superscriptshift);if(e){w=Math.max(w,e.bbox.d+(1/4)*K)}if(i){w=Math.max(w,i.bbox.d+(1/4)*K)}if(e){b.placeBox(e,m+j.bbox.w+z,w)}if(i){b.placeBox(i,0,w)}}else{n=Math.max(n,b.TeX.sub2*M);var x=b.TeX.rule_thickness*M;var I=(l||o).bbox.h,J=(e||i).bbox.d;if(o){I=Math.max(I,o.bbox.h)}if(i){J=Math.max(J,i.bbox.d)}if((w-J)-(I-n)<3*x){n=3*x-w+J+I;C=(4/5)*K-(w-J);if(C>0){w+=C;n-=C}}w=Math.max(w,G.superscriptshift);n=Math.max(n,G.subscriptshift);if(e){b.placeBox(e,m+j.bbox.w+z,w)}if(i){b.placeBox(i,m+L-i.bbox.w,w)}if(l){b.placeBox(l,m+j.bbox.w+z-L,-n)}if(o){b.placeBox(o,m-o.bbox.w,-n)}}}this.HTMLhandleSpace(H);this.HTMLhandleColor(H);return H},HTMLgetScripts:function(p,q){var o,d,e=[];var n=1,g=this.data.length,f=0;for(var h=0;h<4;h+=2){while(n<g&&this.data[n].type!=="mprescripts"){for(var l=h;l<h+2;l++){if(this.data[n]&&this.data[n].type!=="none"){if(!e[l]){e[l]=b.createBox(p);e[l].bbox=this.HTMLemptyBBox({});if(f){b.createBlank(e[l],f);e[l].bbox.w=e[l].bbox.rw=f}}this.data[n].toHTML(e[l]);this.HTMLcombineBBoxes(this.data[n],e[l].bbox)}n++}d=e[h];o=e[h+1];if(d&&o){if(d.bbox.w<o.bbox.w){b.createBlank(d,o.bbox.w-d.bbox.w);d.bbox.w=o.bbox.w;d.bbox.rw=Math.max(d.bbox.w,d.bbox.rw)}else{if(d.bbox.w>o.bbox.w){b.createBlank(o,d.bbox.w-o.bbox.w);o.bbox.w=d.bbox.w;o.bbox.rw=Math.max(o.bbox.w,o.bbox.rw)}}}if(d){f=d.bbox.w}else{if(o){f=o.bbox.w}}}n++;f=0}for(l=0;l<4;l++){if(e[l]){e[l].bbox.w+=q;e[l].bbox.rw=Math.max(e[l].bbox.w,e[l].bbox.rw);this.HTMLcleanBBox(e[l].bbox)}}return e},HTMLstretchH:a.mbase.HTMLstretchH,HTMLstretchV:a.mbase.HTMLstretchV});MathJax.Hub.Startup.signal.Post("HTML-CSS mmultiscripts Ready");MathJax.Ajax.loadComplete(b.autoloadDir+"/mmultiscripts.js")});

