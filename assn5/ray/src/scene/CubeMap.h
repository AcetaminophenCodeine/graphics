#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

class CubeMap
{
	public:
		void setXposMap(TextureMap* tMap_xp) {	cubeFace[0] = tMap_xp; }
		void setXnegMap(TextureMap* tMap_xn) {  cubeFace[1] = tMap_xn; }
		void setYposMap(TextureMap* tMap_yp) {	cubeFace[2] = tMap_yp; }
		void setYnegMap(TextureMap* tMap_yn) {	cubeFace[3] = tMap_yn; }
		void setZposMap(TextureMap* tMap_zp) {  cubeFace[4] = tMap_zp; }
		void setZnegMap(TextureMap* tMap_zn) {	cubeFace[5] = tMap_zn; }

	public:
		TextureMap* cubeFace[6];
};


#endif
