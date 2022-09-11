xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 34;
 1.92915;0.02803;-0.54225;,
 3.43719;0.02803;-0.92351;,
 3.43719;0.94183;-0.00971;,
 1.92916;0.56057;-0.00971;,
 3.43719;0.02803;0.90409;,
 1.92915;0.02803;0.52283;,
 3.43720;-0.88577;-0.00971;,
 1.92915;-0.50451;-0.00971;,
 3.43719;0.02803;-0.92351;,
 1.92915;0.02803;-0.54225;,
 4.99136;0.02803;-1.36697;,
 4.99136;1.38529;-0.00971;,
 4.99136;0.02803;1.34756;,
 4.99136;-1.32923;-0.00971;,
 4.99136;0.02803;-1.36697;,
 6.81031;0.02803;-0.91455;,
 6.81032;0.93287;-0.00971;,
 6.81031;0.02803;0.89513;,
 6.81031;-0.87681;-0.00971;,
 6.81031;0.02803;-0.91455;,
 8.62925;0.02803;-0.46213;,
 8.62925;0.48045;-0.00971;,
 8.62925;0.02803;0.44271;,
 8.62925;-0.42439;-0.00971;,
 8.62925;0.02803;-0.46213;,
 10.44822;0.02803;-0.00971;,
 10.44822;0.02803;-0.00971;,
 10.44822;0.02803;-0.00971;,
 10.44822;0.02803;-0.00971;,
 0.03682;0.02803;-0.00971;,
 1.92915;0.02803;-0.54225;,
 1.92916;0.56057;-0.00971;,
 1.92915;0.02803;0.52283;,
 1.92915;-0.50451;-0.00971;;
 
 24;
 4;3,2,1,0;,
 4;5,4,2,3;,
 4;7,6,4,5;,
 4;9,8,6,7;,
 4;2,11,10,1;,
 4;4,12,11,2;,
 4;6,13,12,4;,
 4;8,14,13,6;,
 4;11,16,15,10;,
 4;12,17,16,11;,
 4;13,18,17,12;,
 4;14,19,18,13;,
 4;16,21,20,15;,
 4;17,22,21,16;,
 4;18,23,22,17;,
 4;19,24,23,18;,
 3;21,25,20;,
 3;22,26,21;,
 3;23,27,22;,
 3;24,28,23;,
 3;31,30,29;,
 3;32,31,29;,
 3;33,32,29;,
 3;30,33,29;;
 
 MeshMaterialList {
  3;
  24;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.194510;0.194510;0.194510;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.160000;0.160000;0.160000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  28;
  -0.185583;0.694825;-0.694822;,
  -0.185583;0.694825;0.694822;,
  -0.185583;-0.694824;0.694823;,
  -0.186890;0.694648;-0.694648;,
  -0.186892;0.694649;0.694647;,
  -0.186891;-0.694649;0.694647;,
  -0.012497;0.707051;-0.707052;,
  -0.012497;0.707053;0.707051;,
  -0.012497;-0.707053;0.707050;,
  0.173217;0.696418;-0.696418;,
  0.173219;0.696418;0.696417;,
  0.173218;-0.696418;0.696417;,
  0.173217;0.696418;-0.696418;,
  0.173217;0.696418;0.696418;,
  0.173216;-0.696418;0.696418;,
  0.173216;0.696418;-0.696418;,
  -0.195167;0.693511;-0.693507;,
  -0.185583;-0.694824;-0.694823;,
  -0.186890;-0.694649;-0.694648;,
  -0.012496;-0.707052;-0.707051;,
  0.173218;-0.696418;-0.696418;,
  0.173217;-0.696418;-0.696418;,
  0.173216;0.696418;0.696418;,
  0.173216;-0.696418;0.696418;,
  0.173216;-0.696418;-0.696418;,
  -0.195167;0.693511;0.693507;,
  -0.195168;-0.693509;0.693509;,
  -0.195168;-0.693509;-0.693509;;
  24;
  4;0,3,3,0;,
  4;1,4,4,1;,
  4;2,5,5,2;,
  4;17,18,18,17;,
  4;3,6,6,3;,
  4;4,7,7,4;,
  4;5,8,8,5;,
  4;18,19,19,18;,
  4;6,9,9,6;,
  4;7,10,10,7;,
  4;8,11,11,8;,
  4;19,20,20,19;,
  4;9,12,12,9;,
  4;10,13,13,10;,
  4;11,14,14,11;,
  4;20,21,21,20;,
  3;12,15,12;,
  3;13,22,13;,
  3;14,23,14;,
  3;21,24,21;,
  3;0,0,16;,
  3;1,1,25;,
  3;2,2,26;,
  3;17,17,27;;
 }
 MeshTextureCoords {
  34;
  0.000000;1.000000;,
  0.000000;0.800000;,
  0.250000;0.800000;,
  0.250000;1.000000;,
  0.500000;0.800000;,
  0.500000;1.000000;,
  0.750000;0.800000;,
  0.750000;1.000000;,
  1.000000;0.800000;,
  1.000000;1.000000;,
  0.000000;0.600000;,
  0.250000;0.600000;,
  0.500000;0.600000;,
  0.750000;0.600000;,
  1.000000;0.600000;,
  0.000000;0.400000;,
  0.250000;0.400000;,
  0.500000;0.400000;,
  0.750000;0.400000;,
  1.000000;0.400000;,
  0.000000;0.200000;,
  0.250000;0.200000;,
  0.500000;0.200000;,
  0.750000;0.200000;,
  1.000000;0.200000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
