/* 
 * File:   Bgshader.h
 * Author: cc
 *
 * Created on 12 août 2011, 21:23
 */

#ifndef BGSHADER_H
#define	BGSHADER_H
namespace ZLX {

    class Bgshader {
    public:
        static void RenderBg();
        static void SetAmplitude(float val);
        static void SetAmplitudeMul(float val);
    };
}
#endif	/* BGSHADER_H */

