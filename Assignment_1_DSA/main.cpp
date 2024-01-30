#include "Restaurant.cpp"
#include <fstream>
int line =1;
int MAXSIZE;

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

void simulate(string filename, imp_res* r)
{
	ifstream ss(filename);
	string str, maxsize, name, energy, num;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
			line++;
    	}
        else if(str == "RED") // RED <NAME> <ENERGY>
        {	

            ss >> name;
            ss >> energy;
			//cout << str << "-" << energy <<" line "<< line << endl;
            r->RED(name, stoi(energy));
			line++;
    	}
    	else if(str == "BLUE") // BLUE <NUM>
    	{
                ss >> num;
				//cout << str << "-" << num <<" line "<< line << endl;
    			r->BLUE(stoi(num));
				line++;
		}
    	else if(str == "PURPLE") // PURPLE
    	{
    			//cout << str <<" line "<< line << endl;
				r->PURPLE();
				line++;
		}
		else if(str == "REVERSAL") // REVERSAL
		{
    			//cout << str <<" line "<< line << endl;
				r->REVERSAL();
				line++;	
		}
    	else if(str == "UNLIMITED_VOID") // UNLIMITED_VOID
     	{   	
    			cout << str <<" line "<< line << endl;
				r->UNLIMITED_VOID();
				line++;
    	}
    	else if(str == "DOMAIN_EXPANSION") // DOMAIN_EXPANSION
    	{
    			//cout << str <<" line "<< line << endl;
				cout << str << " line " << line << endl;
				r->DOMAIN_EXPANSION();
				line++;
    	}
    	else // LIGHT <NUM>
    	{
                ss >> num;
				cout << str <<" " <<num <<" line "<< line << endl;
    			r->LIGHT(stoi(num));
				line++;
    	}
    }
}

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	imp_res* r = new imp_res();
	string fileName = "test.txt";
    simulate(fileName, r);
  	delete r;
	return 0;
	_CrtDumpMemoryLeaks();
}
