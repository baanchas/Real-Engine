#version 330 core
out vec4 color;

in vec3 TexCoords;

uniform samplerCube skybox;

const float PI = 3.14159265359;

void main()
{
    //vec3 normal = normalize(TexCoords);

    //vec3 irradiance = vec3(0.0);

    //vec3 up = vec3(0.0, 1.0, 0.0);
    //vec3 right = cross(up, normal);
    //up = cross(normal, right);

    //float sampleDelta = 0.6;
    //float nrSamples = 0.0;
    //for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    //{
    //    for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
    //    {
    //        // перевод сферических коорд. в декартовы (в касательном пр-ве)
    //        vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    //        // из касательного в мировое пространство
    //        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

    //        irradiance += texture(skybox, sampleVec).rgb * cos(theta) * sin(theta);
    //        nrSamples++;
    //    }
    //}
    //irradiance = PI * irradiance * (1.0 / float(nrSamples));

    //color = vec4(irradiance, 1.0);
	color = texture(skybox, TexCoords);
}