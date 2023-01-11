#version 330 core

struct PositionalLight {
	vec3 Position;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Kc;
	float Kl;
	float Kq;
};

struct DirectionalLight {
	vec3 Position;
	vec3 Direction;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float InnerCutOff;
	float OuterCutOff;
	float Kc;
	float Kl;
	float Kq;
};

struct Material {
	// NOTE(Jovan): Diffuse is used as ambient as well since the light source
	// defines the ambient colour
	sampler2D Kd;
	sampler2D Ks;
	float Shininess;
};

uniform PositionalLight uPointLight11;
uniform PositionalLight uPointLight12;
uniform PositionalLight uPointLight13;
uniform PositionalLight uPointLight14;

uniform PositionalLight uPointLight21;
uniform PositionalLight uPointLight22;
uniform PositionalLight uPointLight23;
uniform PositionalLight uPointLight24;

uniform PositionalLight uPointLight31;
uniform PositionalLight uPointLight32;
uniform PositionalLight uPointLight33;
uniform PositionalLight uPointLight34;

uniform PositionalLight uPointLight41;
uniform PositionalLight uPointLight42;
uniform PositionalLight uPointLight43;
uniform PositionalLight uPointLight44;


uniform DirectionalLight uSpotlight;
uniform DirectionalLight uDirLight;
uniform Material uMaterial;
uniform vec3 uViewPos;

in vec2 UV;
in vec3 vWorldSpaceFragment;
in vec3 vWorldSpaceNormal;

out vec4 FragColor;

void main() {
	vec3 ViewDirection = normalize(uViewPos - vWorldSpaceFragment);
	// NOTE(Jovan): Directional light
	vec3 DirLightVector = normalize(-uDirLight.Direction);
	float DirDiffuse = max(dot(vWorldSpaceNormal, DirLightVector), 0.0f);
	vec3 DirReflectDirection = reflect(-DirLightVector, vWorldSpaceNormal);
	// NOTE(Jovan): 32 is the specular shininess factor. Hardcoded for now
	float DirSpecular = pow(max(dot(ViewDirection, DirReflectDirection), 0.0f), uMaterial.Shininess);

	vec3 DirAmbientColor = uDirLight.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 DirDiffuseColor = uDirLight.Kd * DirDiffuse * vec3(texture(uMaterial.Kd, UV));
	vec3 DirSpecularColor = uDirLight.Ks * DirSpecular * vec3(texture(uMaterial.Ks, UV));
	vec3 DirColor = DirAmbientColor + DirDiffuseColor + DirSpecularColor;

	// NOTE(Jovan): Point light 11
	vec3 PtLightVector11 = normalize(uPointLight11.Position - vWorldSpaceFragment);
	float PtDiffuse11 = max(dot(vWorldSpaceNormal, PtLightVector11), 0.0f);
	vec3 PtReflectDirection11 = reflect(-PtLightVector11, vWorldSpaceNormal);
	float PtSpecular11 = pow(max(dot(ViewDirection, PtReflectDirection11), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor11 = uPointLight11.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor11 = PtDiffuse11 * uPointLight11.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor11 = PtSpecular11 * uPointLight11.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance11 = length(uPointLight11.Position - vWorldSpaceFragment);
	float PtAttenuation11 = 1.0f / (uPointLight11.Kc + uPointLight11.Kl * PtLightDistance11 + uPointLight11.Kq * (PtLightDistance11 * PtLightDistance11));
	vec3 PtColor11 = PtAttenuation11 * (PtAmbientColor11 + PtDiffuseColor11 + PtSpecularColor11);


	// NOTE(Jovan): Point light 12
	vec3 PtLightVector12 = normalize(uPointLight12.Position - vWorldSpaceFragment);
	float PtDiffuse12 = max(dot(vWorldSpaceNormal, PtLightVector12), 0.0f);
	vec3 PtReflectDirection12 = reflect(-PtLightVector12, vWorldSpaceNormal);
	float PtSpecular12 = pow(max(dot(ViewDirection, PtReflectDirection12), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor12 = uPointLight12.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor12 = PtDiffuse12 * uPointLight12.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor12 = PtSpecular12 * uPointLight12.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance12 = length(uPointLight12.Position - vWorldSpaceFragment);
	float PtAttenuation12 = 1.0f / (uPointLight12.Kc + uPointLight12.Kl * PtLightDistance12 + uPointLight12.Kq * (PtLightDistance12 * PtLightDistance12));
	vec3 PtColor12 = PtAttenuation12 * (PtAmbientColor12 + PtDiffuseColor12 + PtSpecularColor12);

	// NOTE(Jovan): Point light 13
	vec3 PtLightVector13 = normalize(uPointLight13.Position - vWorldSpaceFragment);
	float PtDiffuse13 = max(dot(vWorldSpaceNormal, PtLightVector13), 0.0f);
	vec3 PtReflectDirection13 = reflect(-PtLightVector13, vWorldSpaceNormal);
	float PtSpecular13 = pow(max(dot(ViewDirection, PtReflectDirection13), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor13 = uPointLight13.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor13 = PtDiffuse13 * uPointLight13.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor13 = PtSpecular13 * uPointLight13.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance13 = length(uPointLight13.Position - vWorldSpaceFragment);
	float PtAttenuation13 = 1.0f / (uPointLight13.Kc + uPointLight13.Kl * PtLightDistance13 + uPointLight13.Kq * (PtLightDistance13 * PtLightDistance13));
	vec3 PtColor13 = PtAttenuation13 * (PtAmbientColor13 + PtDiffuseColor13 + PtSpecularColor13);

	// NOTE(Jovan): Point light 14
	vec3 PtLightVector14 = normalize(uPointLight14.Position - vWorldSpaceFragment);
	float PtDiffuse14 = max(dot(vWorldSpaceNormal, PtLightVector14), 0.0f);
	vec3 PtReflectDirection14 = reflect(-PtLightVector14, vWorldSpaceNormal);
	float PtSpecular14 = pow(max(dot(ViewDirection, PtReflectDirection14), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor14 = uPointLight14.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor14 = PtDiffuse14 * uPointLight14.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor14 = PtSpecular14 * uPointLight14.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance14 = length(uPointLight14.Position - vWorldSpaceFragment);
	float PtAttenuation14 = 1.0f / (uPointLight14.Kc + uPointLight14.Kl * PtLightDistance14 + uPointLight14.Kq * (PtLightDistance14 * PtLightDistance14));
	vec3 PtColor14 = PtAttenuation14 * (PtAmbientColor14 + PtDiffuseColor14 + PtSpecularColor14);



	// NOTE(Jovan): Point light 21
	vec3 PtLightVector21 = normalize(uPointLight21.Position - vWorldSpaceFragment);
	float PtDiffuse21 = max(dot(vWorldSpaceNormal, PtLightVector21), 0.0f);
	vec3 PtReflectDirection21 = reflect(-PtLightVector21, vWorldSpaceNormal);
	float PtSpecular21 = pow(max(dot(ViewDirection, PtReflectDirection21), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor21 = uPointLight21.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor21 = PtDiffuse21 * uPointLight21.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor21 = PtSpecular21 * uPointLight21.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance21 = length(uPointLight21.Position - vWorldSpaceFragment);
	float PtAttenuation21 = 1.0f / (uPointLight21.Kc + uPointLight21.Kl * PtLightDistance21 + uPointLight21.Kq * (PtLightDistance21 * PtLightDistance21));
	vec3 PtColor21 = PtAttenuation21 * (PtAmbientColor21 + PtDiffuseColor21 + PtSpecularColor21);


	// NOTE(Jovan): Point light 22
	vec3 PtLightVector22 = normalize(uPointLight22.Position - vWorldSpaceFragment);
	float PtDiffuse22 = max(dot(vWorldSpaceNormal, PtLightVector22), 0.0f);
	vec3 PtReflectDirection22 = reflect(-PtLightVector22, vWorldSpaceNormal);
	float PtSpecular22 = pow(max(dot(ViewDirection, PtReflectDirection22), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor22 = uPointLight22.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor22 = PtDiffuse22 * uPointLight22.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor22 = PtSpecular22 * uPointLight22.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance22 = length(uPointLight22.Position - vWorldSpaceFragment);
	float PtAttenuation22 = 1.0f / (uPointLight22.Kc + uPointLight22.Kl * PtLightDistance22 + uPointLight22.Kq * (PtLightDistance22 * PtLightDistance22));
	vec3 PtColor22 = PtAttenuation22 * (PtAmbientColor22 + PtDiffuseColor22 + PtSpecularColor22);

	// NOTE(Jovan): Point light 23
	vec3 PtLightVector23 = normalize(uPointLight23.Position - vWorldSpaceFragment);
	float PtDiffuse23 = max(dot(vWorldSpaceNormal, PtLightVector23), 0.0f);
	vec3 PtReflectDirection23 = reflect(-PtLightVector23, vWorldSpaceNormal);
	float PtSpecular23 = pow(max(dot(ViewDirection, PtReflectDirection23), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor23 = uPointLight23.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor23 = PtDiffuse23 * uPointLight23.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor23 = PtSpecular23 * uPointLight23.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance23 = length(uPointLight23.Position - vWorldSpaceFragment);
	float PtAttenuation23 = 1.0f / (uPointLight23.Kc + uPointLight23.Kl * PtLightDistance23 + uPointLight23.Kq * (PtLightDistance23 * PtLightDistance23));
	vec3 PtColor23 = PtAttenuation23 * (PtAmbientColor23 + PtDiffuseColor23 + PtSpecularColor23);

	// NOTE(Jovan): Point light 24
	vec3 PtLightVector24 = normalize(uPointLight24.Position - vWorldSpaceFragment);
	float PtDiffuse24 = max(dot(vWorldSpaceNormal, PtLightVector24), 0.0f);
	vec3 PtReflectDirection24 = reflect(-PtLightVector24, vWorldSpaceNormal);
	float PtSpecular24 = pow(max(dot(ViewDirection, PtReflectDirection24), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor24 = uPointLight24.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor24 = PtDiffuse24 * uPointLight24.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor24 = PtSpecular24 * uPointLight24.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance24 = length(uPointLight24.Position - vWorldSpaceFragment);
	float PtAttenuation24 = 1.0f / (uPointLight24.Kc + uPointLight24.Kl * PtLightDistance24 + uPointLight24.Kq * (PtLightDistance24 * PtLightDistance24));
	vec3 PtColor24 = PtAttenuation24 * (PtAmbientColor24 + PtDiffuseColor24 + PtSpecularColor24);


	// NOTE(Jovan): Point light 31
	vec3 PtLightVector31 = normalize(uPointLight31.Position - vWorldSpaceFragment);
	float PtDiffuse31 = max(dot(vWorldSpaceNormal, PtLightVector31), 0.0f);
	vec3 PtReflectDirection31 = reflect(-PtLightVector31, vWorldSpaceNormal);
	float PtSpecular31 = pow(max(dot(ViewDirection, PtReflectDirection31), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor31 = uPointLight31.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor31 = PtDiffuse31 * uPointLight31.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor31 = PtSpecular31 * uPointLight31.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance31 = length(uPointLight31.Position - vWorldSpaceFragment);
	float PtAttenuation31 = 1.0f / (uPointLight31.Kc + uPointLight31.Kl * PtLightDistance31 + uPointLight31.Kq * (PtLightDistance31 * PtLightDistance31));
	vec3 PtColor31 = PtAttenuation31 * (PtAmbientColor31 + PtDiffuseColor31 + PtSpecularColor31);


	// NOTE(Jovan): Point light 32
	vec3 PtLightVector32 = normalize(uPointLight32.Position - vWorldSpaceFragment);
	float PtDiffuse32 = max(dot(vWorldSpaceNormal, PtLightVector32), 0.0f);
	vec3 PtReflectDirection32 = reflect(-PtLightVector32, vWorldSpaceNormal);
	float PtSpecular32 = pow(max(dot(ViewDirection, PtReflectDirection32), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor32 = uPointLight32.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor32 = PtDiffuse32 * uPointLight32.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor32 = PtSpecular32 * uPointLight32.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance32 = length(uPointLight32.Position - vWorldSpaceFragment);
	float PtAttenuation32 = 1.0f / (uPointLight32.Kc + uPointLight32.Kl * PtLightDistance32 + uPointLight32.Kq * (PtLightDistance32 * PtLightDistance32));
	vec3 PtColor32 = PtAttenuation32 * (PtAmbientColor32 + PtDiffuseColor32 + PtSpecularColor32);

	// NOTE(Jovan): Point light 33
	vec3 PtLightVector33 = normalize(uPointLight33.Position - vWorldSpaceFragment);
	float PtDiffuse33 = max(dot(vWorldSpaceNormal, PtLightVector33), 0.0f);
	vec3 PtReflectDirection33 = reflect(-PtLightVector33, vWorldSpaceNormal);
	float PtSpecular33 = pow(max(dot(ViewDirection, PtReflectDirection33), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor33 = uPointLight33.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor33 = PtDiffuse33 * uPointLight33.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor33 = PtSpecular33 * uPointLight33.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance33 = length(uPointLight33.Position - vWorldSpaceFragment);
	float PtAttenuation33 = 1.0f / (uPointLight33.Kc + uPointLight33.Kl * PtLightDistance33 + uPointLight33.Kq * (PtLightDistance33 * PtLightDistance33));
	vec3 PtColor33 = PtAttenuation33 * (PtAmbientColor33 + PtDiffuseColor33 + PtSpecularColor33);

	// NOTE(Jovan): Point light 34
	vec3 PtLightVector34 = normalize(uPointLight34.Position - vWorldSpaceFragment);
	float PtDiffuse34 = max(dot(vWorldSpaceNormal, PtLightVector34), 0.0f);
	vec3 PtReflectDirection34 = reflect(-PtLightVector34, vWorldSpaceNormal);
	float PtSpecular34 = pow(max(dot(ViewDirection, PtReflectDirection34), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor34 = uPointLight34.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor34 = PtDiffuse34 * uPointLight34.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor34 = PtSpecular34 * uPointLight34.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance34 = length(uPointLight34.Position - vWorldSpaceFragment);
	float PtAttenuation34 = 1.0f / (uPointLight34.Kc + uPointLight34.Kl * PtLightDistance34 + uPointLight34.Kq * (PtLightDistance34 * PtLightDistance34));
	vec3 PtColor34 = PtAttenuation34 * (PtAmbientColor34 + PtDiffuseColor34 + PtSpecularColor34);



	// NOTE(Jovan): Point light 41
	vec3 PtLightVector41 = normalize(uPointLight41.Position - vWorldSpaceFragment);
	float PtDiffuse41 = max(dot(vWorldSpaceNormal, PtLightVector41), 0.0f);
	vec3 PtReflectDirection41 = reflect(-PtLightVector41, vWorldSpaceNormal);
	float PtSpecular41 = pow(max(dot(ViewDirection, PtReflectDirection41), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor41 = uPointLight41.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor41 = PtDiffuse41 * uPointLight41.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor41 = PtSpecular41 * uPointLight41.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance41 = length(uPointLight41.Position - vWorldSpaceFragment);
	float PtAttenuation41 = 1.0f / (uPointLight41.Kc + uPointLight41.Kl * PtLightDistance41 + uPointLight41.Kq * (PtLightDistance41 * PtLightDistance41));
	vec3 PtColor41 = PtAttenuation41 * (PtAmbientColor41 + PtDiffuseColor41 + PtSpecularColor41);


	// NOTE(Jovan): Point light 42
	vec3 PtLightVector42 = normalize(uPointLight42.Position - vWorldSpaceFragment);
	float PtDiffuse42 = max(dot(vWorldSpaceNormal, PtLightVector42), 0.0f);
	vec3 PtReflectDirection42 = reflect(-PtLightVector42, vWorldSpaceNormal);
	float PtSpecular42 = pow(max(dot(ViewDirection, PtReflectDirection42), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor42 = uPointLight42.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor42 = PtDiffuse42 * uPointLight42.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor42 = PtSpecular42 * uPointLight42.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance42 = length(uPointLight42.Position - vWorldSpaceFragment);
	float PtAttenuation42 = 1.0f / (uPointLight42.Kc + uPointLight42.Kl * PtLightDistance42 + uPointLight42.Kq * (PtLightDistance42 * PtLightDistance42));
	vec3 PtColor42 = PtAttenuation42 * (PtAmbientColor42 + PtDiffuseColor42 + PtSpecularColor42);

	// NOTE(Jovan): Point light 43
	vec3 PtLightVector43 = normalize(uPointLight43.Position - vWorldSpaceFragment);
	float PtDiffuse43 = max(dot(vWorldSpaceNormal, PtLightVector43), 0.0f);
	vec3 PtReflectDirection43 = reflect(-PtLightVector43, vWorldSpaceNormal);
	float PtSpecular43 = pow(max(dot(ViewDirection, PtReflectDirection43), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor43 = uPointLight43.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor43 = PtDiffuse43 * uPointLight43.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor43 = PtSpecular43 * uPointLight43.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance43 = length(uPointLight43.Position - vWorldSpaceFragment);
	float PtAttenuation43 = 1.0f / (uPointLight43.Kc + uPointLight43.Kl * PtLightDistance43 + uPointLight43.Kq * (PtLightDistance43 * PtLightDistance43));
	vec3 PtColor43 = PtAttenuation43 * (PtAmbientColor43 + PtDiffuseColor43 + PtSpecularColor43);

	// NOTE(Jovan): Point light 44
	vec3 PtLightVector44 = normalize(uPointLight44.Position - vWorldSpaceFragment);
	float PtDiffuse44 = max(dot(vWorldSpaceNormal, PtLightVector44), 0.0f);
	vec3 PtReflectDirection44 = reflect(-PtLightVector44, vWorldSpaceNormal);
	float PtSpecular44 = pow(max(dot(ViewDirection, PtReflectDirection44), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor44 = uPointLight44.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 PtDiffuseColor44 = PtDiffuse44 * uPointLight44.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 PtSpecularColor44 = PtSpecular44 * uPointLight44.Ks * vec3(texture(uMaterial.Ks, UV));

	float PtLightDistance44 = length(uPointLight44.Position - vWorldSpaceFragment);
	float PtAttenuation44 = 1.0f / (uPointLight44.Kc + uPointLight44.Kl * PtLightDistance44 + uPointLight44.Kq * (PtLightDistance44 * PtLightDistance44));
	vec3 PtColor44 = PtAttenuation44 * (PtAmbientColor44 + PtDiffuseColor44 + PtSpecularColor44);




	// NOTE(Jovan): Spotlight
	vec3 SpotlightVector = normalize(uSpotlight.Position - vWorldSpaceFragment);

	float SpotDiffuse = max(dot(vWorldSpaceNormal, SpotlightVector), 0.0f);
	vec3 SpotReflectDirection = reflect(-SpotlightVector, vWorldSpaceNormal);
	float SpotSpecular = pow(max(dot(ViewDirection, SpotReflectDirection), 0.0f), uMaterial.Shininess);

	vec3 SpotAmbientColor = uSpotlight.Ka * vec3(texture(uMaterial.Kd, UV));
	vec3 SpotDiffuseColor = SpotDiffuse * uSpotlight.Kd * vec3(texture(uMaterial.Kd, UV));
	vec3 SpotSpecularColor = SpotSpecular * uSpotlight.Ks * vec3(texture(uMaterial.Ks, UV));

	float SpotlightDistance = length(uSpotlight.Position - vWorldSpaceFragment);
	float SpotAttenuation = 1.0f / (uSpotlight.Kc + uSpotlight.Kl * SpotlightDistance + uSpotlight.Kq * (SpotlightDistance * SpotlightDistance));

	float Theta = dot(SpotlightVector, normalize(-uSpotlight.Direction));
	float Epsilon = uSpotlight.InnerCutOff - uSpotlight.OuterCutOff;
	float SpotIntensity = clamp((Theta - uSpotlight.OuterCutOff) / Epsilon, 0.0f, 1.0f);
	vec3 SpotColor = SpotAttenuation * (SpotAmbientColor + SpotDiffuseColor + SpotSpecularColor);
	
	vec3 FinalColor = DirColor + PtColor11 + PtColor12 + PtColor13 + PtColor14 + PtColor21 + PtColor22 + PtColor23 + PtColor24 + PtColor31 + PtColor32 + PtColor33 + PtColor34 + PtColor41 + PtColor42 + PtColor43 + PtColor44 + SpotColor;
	FragColor = vec4(FinalColor, 1.0f);
}