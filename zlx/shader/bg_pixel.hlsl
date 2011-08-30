float rate: register(c1);
float amplitude : register(c2);
float amplitude_mul : register(c3);

float fTime : register(c0);

float ComputeVagues(float2 texCoord: TEXCOORD0,float offset){

   float time = fTime/10.0f;
   //float time = 10.0f;
   float x = rate * texCoord.x + time + (offset * 100);
   
   float res = 1.0f - (abs(texCoord.y*(amplitude*amplitude_mul)+(sin(x)*amplitude)));
      
   return res * (  cos(texCoord.x)/40   );

}

float4 main( float4 inDiffuse: COLOR0, float2 texCoord: TEXCOORD0 ) : COLOR0
{
   float vagues0 =  ComputeVagues( texCoord, 0.0f );
   
   texCoord.y = texCoord.y * 2; //2;//cos(fTime);
   
   float vagues1 =  ComputeVagues( texCoord, sin(30.0f) ); 
   float vagues2 =  ComputeVagues( texCoord, cos(10.0f) );
 
   float4 vagues_exterieur = (1.5f+vagues0)*float4(1,0,0,1); // RGBA
   float4 vagues_blanc = (1.5f+vagues1)*float4(0,0,1,1);
   float4 vagues_gr = (1.5f+vagues2)*float4(0,1,0,1);
   float4 vague_principale = (vagues_gr + vagues_exterieur + vagues_blanc);
   
   return  sin(vague_principale ) ;//+ ((1.5f+(ComputeVagues( texCoord, 1.0f )))*float4(1,0,1,1));
}