#include "CubeMapChooser.h"
#include "../scene/CubeMap.h"
#include "../scene/material.h"
#include "../ui/GraphicalUI.h"
#include <iostream>


/*
direct.h is a C/C++ header file provided by 
Microsoft Windows, which contains functions for 
manipulating file system directories. Some POSIX 
functions that do similar things are in unistd.h.
*/

#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

CubeMapChooser::CubeMapChooser() 
{
		//	Fl_Menu_Window* w;
		w = new Fl_Menu_Window(395, 355, "Load Cubemap");
		w -> user_data((void*)(this));


		//	Fl_File_Input* fi[6];
		fi[0] = new Fl_File_Input(20,  25, 310, 35);
		fi[1] = new Fl_File_Input(20,  71, 310, 35);
		fi[2] = new Fl_File_Input(20, 117, 310, 35);
		fi[3] = new Fl_File_Input(20, 163, 310, 35);
		fi[4] = new Fl_File_Input(20, 209, 310, 35);
		fi[5] = new Fl_File_Input(20, 255, 310, 35);


		//	fptrarray cb_fi[6];
		cb_fi[0] =  &cb_xpi;	   //	static void cb_xpi (Fl_Widget* o, void* v);
		cb_fi[1] =  &cb_xni;	   //	static void cb_xni (Fl_Widget* o, void* v);
		cb_fi[2] =  &cb_ypi;	   //	static void cb_ypi (Fl_Widget* o, void* v);
		cb_fi[3] =  &cb_yni;	   //	static void cb_yni (Fl_Widget* o, void* v);
		cb_fi[4] =  &cb_zpi;	   //	static void cb_zpi (Fl_Widget* o, void* v);
		cb_fi[5] =  &cb_zni;	   //	static void cb_zni (Fl_Widget* o, void* v);


		//	FILENAME_MAX. Maximum length of file names
		char curPath[FILENAME_MAX];

		//#ifdef WIN32
		//    #include <direct.h>
   	    //			#define GetCurrentDir _getcwd
		//    #include <unistd.h>
		//			#define GetCurrentDir getcwd
		//#endif

		if (!GetCurrentDir(curPath, sizeof(curPath))) 
			curPath[0] = 0;


		string fileName = std::string(curPath);


		//	Fl_File_Input* fi[6];
		for (int i = 0; i < 6; i++) 
		{
			fi[i]->user_data((void*)(this));
			//	fptrarray cb_fi[6];
			fi[i]->callback(cb_fi[i]);
			fi[i]->value(curPath);
			//	std::string fn[6];
			fn[i] = fileName;
		}


		// fptrarray cb_fb[6];
		cb_fb[0] =  &cb_xpb;			// static void cb_xpb (Fl_Widget* o, void* v);
		cb_fb[1] =  &cb_xnb;			// static void cb_xnb (Fl_Widget* o, void* v);
		cb_fb[2] =  &cb_ypb;			// static void cb_ypb (Fl_Widget* o, void* v);
		cb_fb[3] =  &cb_ynb;			// static void cb_ynb (Fl_Widget* o, void* v);
		cb_fb[4] =  &cb_zpb;			// static void cb_zpb (Fl_Widget* o, void* v);
		cb_fb[5] =  &cb_znb;			// static void cb_znb (Fl_Widget* o, void* v);


		// std::string btnMsg[6];
		btnMsg[0] = "Positive X Texture Map";
		btnMsg[1] = "Negative X Texture Map";
		btnMsg[2] = "Positive Y Texture Map";
		btnMsg[3] = "Negative Y Texture Map";
		btnMsg[4] = "Positive Z Texture Map";
		btnMsg[5] = "Negative Z Texture Map";


		// Fl_Light_Button* fb[6];
		fb[0] = new Fl_Light_Button(340, 35,  40, 25, "+X");
		fb[1] = new Fl_Light_Button(340, 81,  40, 25, "-X");
		fb[2] = new Fl_Light_Button(340, 127, 40, 25, "+Y");
		fb[3] = new Fl_Light_Button(340, 173, 40, 25, "-Y");
		fb[4] = new Fl_Light_Button(340, 219, 40, 25, "+Z");
		fb[5] = new Fl_Light_Button(340, 265, 40, 25, "-Z");


		// Fl_Light_Button* fb[6];
		for (int i = 0; i < 6; i++) 
		{
			fb[i]->user_data((void*)(this));
			fb[i]->callback(cb_fb[i]);
		}

		//	Fl_Button* ok;
		ok = new Fl_Button(250, 310, 60, 25, "OK");
		ok->user_data((void*)(this));
		ok->callback(cb_ok);

		// Fl_Button* cancel;
		cancel = new Fl_Button(320, 310, 60, 25, "Cancel");
		cancel->user_data((void*)(this));
		cancel->callback(cb_cancel);

		//	Fl_Menu_Window* w;
		w->end();
		w->set_modal();
}

//	Fl_Menu_Window* w;
void CubeMapChooser::show() {
	w->show();
}

//	Fl_Menu_Window* w;
void CubeMapChooser::hide() {
	w->hide();
}


void CubeMapChooser::cb_cancel(Fl_Widget* o, void* v) {
	o->parent()->hide();
}


void CubeMapChooser::cb_ok(Fl_Widget* o, void* v) 
{
	CubeMapChooser* ch = (CubeMapChooser*)(o->parent()->user_data());
	int allGreen = 0;

	// Fl_Light_Button* fb[6];
	while (ch->fb[allGreen]->selection_color() == FL_GREEN) 
								allGreen++;

	if (allGreen == 6) 
	{
		CubeMap* cm = 0;

		//	GraphicalUI* caller;
		if (ch->caller->getRayTracer()->haveCubeMap())
			cm = ch->caller->getRayTracer()->getCubeMap();
		else 
		{
			cm = new CubeMap();
			ch->caller->getRayTracer()->setCubeMap(cm);
		}

		cm->setXposMap(ch->cubeFace[0]);
		cm->setXnegMap(ch->cubeFace[1]);
		cm->setYposMap(ch->cubeFace[2]);
		cm->setYnegMap(ch->cubeFace[3]);
		cm->setZposMap(ch->cubeFace[4]);
		cm->setZnegMap(ch->cubeFace[5]);
		ch->caller->setCubeMap(true);
		ch->caller->useCubeMap(true);
		ch->caller->m_filterSlider->activate();
		ch->caller->m_cubeMapCheckButton->activate();
		ch->caller->m_cubeMapCheckButton->value(1);
	}

	o->parent()->hide();
}


void CubeMapChooser::cb_xpi(Fl_Widget* o, void* v) {
	cb_ffi (o, 0);
}


void CubeMapChooser::cb_xpb(Fl_Widget* o, void* v) {
	cb_ffb (o, 0);
}


void CubeMapChooser::cb_xni(Fl_Widget* o, void* v) {
	cb_ffi (o, 1);
}


void CubeMapChooser::cb_xnb(Fl_Widget* o, void* v) {
	cb_ffb (o, 1);
}


void CubeMapChooser::cb_ypi(Fl_Widget* o, void* v) {
	cb_ffi (o, 2);
}


void CubeMapChooser::cb_ypb(Fl_Widget* o, void* v) {
	cb_ffb (o, 2);
}


void CubeMapChooser::cb_yni(Fl_Widget* o, void* v) {
	cb_ffi (o, 3);
}


void CubeMapChooser::cb_ynb(Fl_Widget* o, void* v) {
	cb_ffb (o, 3);
}


void CubeMapChooser::cb_zpi(Fl_Widget* o, void* v) {
	cb_ffi (o, 4);
}


void CubeMapChooser::cb_zpb(Fl_Widget* o, void* v) {
	cb_ffb (o, 4);
}


void CubeMapChooser::cb_zni(Fl_Widget* o, void* v) {
	cb_ffi (o, 5);
}


void CubeMapChooser::cb_znb(Fl_Widget* o, void* v) {
	cb_ffb (o, 5);
}


void CubeMapChooser::cb_ffi(Fl_Widget* o, int i) 
{
	CubeMapChooser* ch = (CubeMapChooser*)(o->parent()->user_data());
	try { ch->cubeFace[i] = new TextureMap(ch->fi[i]->value()); }
	catch (TextureMapException &xcpt) {
		ch->fb[i]->selection_color(FL_RED);
		ch->fb[i]->value(0);
		ch->fb[i]->value(1);
		std::cerr << xcpt.message() << std::endl;
		std::string msg("Error: could not open file: ");
		msg.append(ch->fi[i]->value());
		ch->caller->alert(msg);
		return;
	}
	
	std::string fN = std::string(ch->fi[i]->value());
	std::string pN = fN.substr(0,fN.find_last_of("/"));
	for (int j = 0; j < 6; j++) {
		if (ch->fb[j]->selection_color() != FL_GREEN) {
			ch->fi[j]->value(pN.c_str());
			ch->fn[j] = pN;
		}
	}
	ch->fi[i]->value(fN.c_str());
	ch->fn[i] = fN;
	ch->fb[i]->selection_color(FL_GREEN);
	ch->fb[i]->value(0);
	ch->fb[i]->value(1);
}


void CubeMapChooser::cb_ffb(Fl_Widget* o, int i) {
	CubeMapChooser* ch = (CubeMapChooser*)(o->parent()->user_data());
	if (char* curPath = fl_file_chooser(ch->btnMsg[i].c_str(),  ".bmp or .png (*.{bmp,png})", ch->fn[i].c_str(), 0)) 
	{
		try { ch->cubeFace[i] = new TextureMap(curPath); }
		catch (TextureMapException &xcpt) {
			ch->fb[i]->selection_color(FL_RED);
			ch->fb[i]->value(0);
			ch->fb[i]->value(1);
			std::cerr << xcpt.message() << std::endl;
			std::string msg("Error: could not open file: ");
			msg.append(ch->fi[i]->value());
			ch->caller->alert(msg);
			return;
		}
		std::string fN = std::string(curPath);
		std::string pN = fN.substr(0, fN.find_last_of("/"));
		for (int j = 0; j < 6; j++) {
			if (ch->fn[j].substr(0, ch->fn[j].find_last_of("/")) != pN) {
				ch->fi[j]->value(pN.c_str());
				ch->fn[j] = pN;
			}
		}
		ch->fn[i] = fN;
		ch->fi[i]->value(curPath);
		ch->fb[i]->selection_color(FL_GREEN);
	}
	ch->fb[i]->value(0);
	ch->fb[i]->value(1);
}
