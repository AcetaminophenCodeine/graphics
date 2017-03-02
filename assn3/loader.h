#ifndef __LOADER_H__
#define __LOADER_H__
#include <iostream>
#include <fstream>
#include <cstring>
#include "geom.h"
using namespace std;
#define LINE_SIZE    1024
#define TOKEN_LENGTH 8

enum TokenID { T_NONE=-1, T_VERT, T_FACE}; 


struct TokenPair 
{
	char strval[TOKEN_LENGTH];
	TokenID  tokID;
	bool operator==(const TokenPair &rhs)  const
	{
		return strcmp(strval,rhs.strval) == 0 && tokID == rhs.tokID;
	}
	bool operator!=(const TokenPair &rhs) const
	{
		return !(*this==rhs);
	}
};



class TrimeshLoader
{
	public:

		static TokenPair EMPTY_PAIR;

		static TokenPair tokenMap[];   // array of TokenPair struct

		static char TOK_SEPS[];        // array of token seperators (like "\t") 

		TokenPair * tokenMatch(char * searchedToken)
		{
			if(searchedToken == NULL) 
				return 0;

			TokenPair *tokenPairPtr = &tokenMap[0];

			for(; *tokenPairPtr != EMPTY_PAIR && strcmp(tokenPairPtr->strval,searchedToken) != 0; tokenPairPtr++);

			if(*tokenPairPtr == EMPTY_PAIR) 
					tokenPairPtr = 0;

			return tokenPairPtr;
		}

		void loadOBJ(const char * objfile, Trimesh * pmesh)
		{
			//cout << "loadObject called" << endl; 
			std::ifstream inputFile;
			char line[LINE_SIZE];
			char * tok;
			inputFile.open(objfile);

			// while objectfile still has data
			while(!inputFile.eof())
			{
				inputFile.getline (line, LINE_SIZE);
				tok = strtok (line, TOK_SEPS);
				TokenPair * tokenPairPtr = tokenMatch(tok);

				if(tokenPairPtr != NULL)
				{
					switch(tokenPairPtr->tokID)
					{
						case T_VERT: 
							processVertex(tok,pmesh); 
							break;

						case T_FACE: 
							processFace(tok,pmesh); 
							break;
						
						default: 
							processSkip(tok); 
							break;
					}
				}
			}

			inputFile.close();
		}

		int readFloats(char * vertexComponentToken, float * buf, int bufSize)
		{
			int i = 0; 

			while((vertexComponentToken = strtok(0, TOK_SEPS)) != 0 && i < bufSize)
			{
				// atof converts string to float
				buf[i++] = atof(vertexComponentToken);
			}

			return i;
		}

		int readInts(char * faceComponentToken, int * buf, int bufSize)
		{
			int i = 0;

			while((faceComponentToken = strtok(0, TOK_SEPS)) != 0 && i < bufSize)
			{
				// atoi converst string to integer
				buf[i++] = atoi(faceComponentToken);
			}
			return i;
		}

		void processSkip(char * tok)
		{}


		void processVertex(char *vertexline, Trimesh * pmsh)
		{
			float values[3];
			int count = readFloats(vertexline, values, 3);	 

			if(count >= 3) 
				pmsh->addVertex(values);
		}

		bool processFace(char *facelines, Trimesh * pmsh)
		{
			int ids[256]; 
			int count = readInts(facelines, ids, 256);

			if(count >= 3)
			{
				int tri[3] = {ids[0]-1, ids[1]-1, ids[2]-1};

				pmsh->addFace(tri);

				for(int i=3; i < count; i++)
				{
					tri[1] = tri[2];
					tri[2] = ids[i]-1;
					pmsh->addFace(tri);
				}
			}

			return true;
		}
};

TokenPair TrimeshLoader::EMPTY_PAIR={"",T_NONE};

TokenPair TrimeshLoader::tokenMap[] = { 
	{"v", T_VERT}, {"f",T_FACE}, 
	EMPTY_PAIR	
};

char TrimeshLoader::TOK_SEPS[] = " \t";
#endif
