#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform sampler2D tex_palette;
uniform sampler2D tex_noise0;
uniform sampler2D tex_noise1;
uniform vec2 resolution;
uniform float time;

float threshold = 0.777;

vec4 colora = vec4(1.0, 1.0, 1.0, 1.0);
vec4 colorb = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec2 uv = fragTexCoord;
	uv.y *= -1.0;
	vec4 color = texture2D(texture0, uv);
	vec4 noise0 = texture2D(tex_noise0, uv);
	gl_FragColor = vec4( color );
/*
	
	
	vec2 noise0d = texture2D(tex_noise0, uv);
	noise0d *= 0.7;

    
    float a = distance(noise0d.r, 1.0);
    float alpha = smoothstep(threshold - 0.1, threshold + 0.1, a);

    

    float wavex = sin(uv.x * 40.0 + time * 0.07) * 0.5 + 1.0;
    float wavey = cos(uv.y * 60.0 + time * 0.1) * 0.5 + 1.0;
    vec2 noise00uv = uv - vec2(0.0 + wavey * 0.02 + sin(time * 0.05), time * 0.02+ wavex * 0.03);
    noise00uv *= 0.7;
    vec4 noise00 = texture2D(tex_noise0, noise00uv);
    vec4 noise0 = texture2D(tex_noise0, noise00.xy - vec2(wavex * 0.1, time * 0.1));
    vec4 noise1  = texture2D(tex_noise1, uv * 0.7 + noise00.xy * 0.2 - vec2(0.0, time * 0.01));
*/

/* 
    float rim0 = 1.0 - distance(alpha, threshold);
    float rim1 = smoothstep(threshold - 0.04, threshold + 0.05, rim0 * alpha);
    
    color = color * mix(colorb, colora, rim1);
    //color *= alpha;
    
    gl_FragColor = vec4(color);
*/
}