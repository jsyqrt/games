uniform vec2 frag_LightOrigin;	
uniform vec3 frag_LightColor; 	
uniform float frag_LightAttenuation;	
uniform vec2 frag_ScreenResolution; 
uniform sampler2D texture;
void main()
{		
    vec2 baseDistance =  gl_FragCoord.xy; 
    baseDistance.y = frag_ScreenResolution.y-baseDistance.y; 
    float d = length(frag_LightOrigin - baseDistance); 
    float a = 1.0/(frag_LightAttenuation * d);	
    vec4 color = vec4(a,a,a,1.0) * vec4(frag_LightColor, 1.0);  
    vec4 t = texture2D(texture, gl_TexCoord[0].xy);
    if (t[0]>color[0]) color[0]=t[0];
    if (t[1]>color[1]) color[1]=t[1];
    if (t[2]>color[2]) color[2]=t[2];
    gl_FragColor=color; 
}
