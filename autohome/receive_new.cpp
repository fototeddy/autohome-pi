/*	
	Original Code from CurlyMo (int main () )

	function for interpreting signals from Conrad KW9010 from BastardOp


	Copyright 2012 CurlyMo
	
	This file is part of the Raspberry Pi 433.92Mhz transceiver.

    Raspberry Pi 433.92Mhz transceiver is free software: you can redistribute 
	it and/or modify it under the terms of the GNU General Public License as 
	published by the Free Software Foundation, either version 3 of the License, 
	or (at your option) any later version.

    Raspberry Pi 433.92Mhz transceiver is distributed in the hope that it will 
	be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Raspberry Pi 433.92Mhz transceiver. If not, see 
	<http://www.gnu.org/licenses/>
*/

//#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
//#include <bitset>
//#include <limits>
#include <time.h>
#include <algorithm>
#include <iterator>
#include <time.h>

using namespace std;

string code;
string push;

vector<string> bin;
vector<string> acode;
vector<string> tempdata;
vector<string> ccode;

time_t old_time;
time_t new_time;

float pushed;
long int wert = 0;
int speed = 50;
int end = 100;
int start = 30;
int marge = 5;
int debug = 0;
int bit = 2;
int type = 0;
int noendl = 0;

double diff_time;

string old_id;
string old_channel;

string itos(int i) {
    ostringstream s;
    s << i;
    return s.str();
}


void makebin(){
	int i=0;
	for(i=0;i<acode.size();++i) {
		if(atoi(acode[i].c_str()) > 10 && atoi(acode[i].c_str()) < 39){
			bin.push_back ("0");
		} else if (atoi(acode[i].c_str()) > 39) {
			bin.push_back("1");
		}
	}

}

void binaryParts(int start, int stop) {
	int i=0;
	push="" ;
	for(i=start;i<=stop;++i){
		push.append(bin[i]);
	}
}

void makedata(){
	binaryParts(0,2);
	tempdata.push_back (push);
	
	binaryParts(3,4);
        tempdata.push_back (push);
	
	binaryParts(11,22);
	reverse(push.begin(), push.end());
	tempdata.push_back (push);
	pushed = stoi(push,nullptr,2);        
	tempdata.push_back (to_string(pushed/10));
	
	binaryParts(23,29);
        reverse(push.begin(), push.end());
        tempdata.push_back (push);
	pushed = stoi(push,nullptr,2);        
        tempdata.push_back (to_string(pushed-28));
}

vector<string> explode( const string &delimiter, const string &str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}



int main(int argc, char **argv) { 
	int pin_in = 2;
	int read = 0;
	int one = 0;
	int zero = 0;
	int opt = 0;
	//int noCalc = 0;
	printf("Start\n");

code = "2;25;25;6;25;5;53;5;53;5;25;6;26;6;25;5;26;6;24;5;50;5;51;6;24;6;25;6;26;6;25;5;54;5;55;5;26;5;26;6;54;5;26;5;24;5;24;5;24;5;25;5;25;6;52;5;26;5;27;6;58;6;57;5;55;5;26;5;54;6;51;5;";
	
			acode = explode(";",code);
				cout << "Code length:\t" << acode.size() << endl;
			
			switch((int)acode.size()) {
				case 73:
					type = 2;
                break;
                case 72:
                    type = 2
				break;
				default:
					type = 0;
				break;
			}			
			if(type > 0) {
				
				if(type == 2){
					makebin();
					if(debug == 1) {
						cout << "Binary length:\t" << bin.size() << endl;
						cout << "Binary:\t";
						copy(bin.begin(), bin.end(), ostream_iterator<string>(cout));
						cout << endl;
					}
					switch((int)bin.size()) {
						case 35:
							type = 1;
						break;
                        case 36:
                            type = 1;
                        break;
						default:
							type = 0;
						break;
					}

					if(type == 1) {
						makedata();
                        time(&new_time);
                        diff_time = difftime(new_time, old_time);
                        if (old_id != tempdata[0] && old_channel != tempdata[1]) {
                            cout << "different sender" << endl;
                            cout << "id " << tempdata[0] << endl;
                            cout << "channel " << tempdata[1] << endl;
                            cout << "temp bin " <<tempdata[2] << endl;
                            cout << "temp " << tempdata[3] << endl;
                            cout << "humi bin " << tempdata[4] << endl;
                            cout << "humi " << tempdata[5] << endl;
                            old_id = tempdata[0];
                            old_channel = tempdata[1];
                            old_time = new_time;

                        } else if (diff_time > 10) {
                            
                            cout << "different time" << endl;
                            cout << "id " << tempdata[0] << endl;
                            cout << "channel " << tempdata[1] << endl;
                            cout << "temp bin " <<tempdata[2] << endl;
                            cout << "temp " << tempdata[3] << endl;
                            cout << "humi bin " << tempdata[4] << endl;
                            cout << "humi " << tempdata[5] << endl;
                            old_id = tempdata[0];
                            old_channel = tempdata[1];
                            old_time = new_time;
                        }
                        
					}
				
				}	
			}
			type = 0;
			bin.clear();
			code.clear();
			acode.clear();							
			one=0;
			ccode.clear();
			zero=0;
		
	
}