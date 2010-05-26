uniform sampler2D text;
uniform float n;
uniform vec3 avgLuminance;
uniform float imgSize;

void main()
{
   if(dFdx(gl_TexCoord[0].s) < n)
   {
      if(all( lessThan(  gl_TexCoord[0].st, vec2(n,n)  ) ))
      {
         float ds = 1.0/imgSize;
         float dt = 1.0/imgSize;
//         float ds = dFdx(gl_TexCoord[0].s);
//         float dt = dFdy(gl_TexCoord[0].t);

         vec2 initialCoord = (gl_TexCoord[0].st*2.0);
         vec4 lum = vec4(0.27, 0.67, 0.06, 1.0);
         vec4 tex1 = texture2D(text, initialCoord);
         vec4 tex2 = texture2D(text, initialCoord + vec2(ds, 0));
         vec4 tex3 = texture2D(text, initialCoord + vec2(0, dt));
         vec4 tex4 = texture2D(text, initialCoord + vec2(ds, dt));
         gl_FragColor = (tex1+tex2+tex3+tex4)/4.0;
      }else
         discard;
   }
   else
   {
      vec4 tex = texture2D(text, gl_TexCoord[0].st);
      gl_FragColor = tex;
   }
}


//   {
//      gl_FragColor = vec4(1,0,0,1);
//   }else  gl_FragColor = vec4(0,0,1,1);
