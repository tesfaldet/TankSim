varying vec3 N;
varying vec3 L;
varying vec3 eyePosition;
uniform sampler2D texMap;

void main ()
{
  vec3 R = normalize(-reflect(L,N));
  vec3 E = normalize(-eyePosition);
  
  //calculate Ambient Term:
  vec4 Iamb = gl_FrontLightProduct[0].ambient;
  
  //calculate Diffuse Term:
  vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
  Idiff = clamp(Idiff, 0.0, 1.0);
  
  // calculate Specular Term:
  vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
  Ispec = clamp(Ispec, 0.0, 1.0);
  
  vec4 texColor = texture2D(texMap, gl_TexCoord[0].st);
  
  // write Total Color:
  gl_FragColor = (gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec) * texColor;
}