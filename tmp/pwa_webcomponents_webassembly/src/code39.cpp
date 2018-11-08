/*
GPL v3 License

    This source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    It is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <cstdlib>
#include <emscripten/bind.h>

using namespace emscripten;

class Code39 {

public:
  Code39()
  {
	inputData="12345678";
	checkDigit=1;
  }

  Code39(std::string inputData, int checkDigit)
    : checkDigit(checkDigit)
    , inputData(inputData)
  {}

  std::string encode() {  

	std::string filteredData=filterInput(inputData);

	int filteredlength = filteredData.length();
	std::string result;
	if (checkDigit==1)
		result="*"+filteredData+generateCheckDigit(filteredData)+"*";
	else
		result="*"+filteredData+"*";

	std::string mappedResult;
	for (int x=0;x<result.length();x++)
	{	       
	        mappedResult=mappedResult+"&#"+std::to_string((unsigned char)result[x])+";";	
    	}
	result=mappedResult;

	human_readable_text=result;
	return result;
  }
  int getCheckDigit() const { return checkDigit; }
  void setCheckDigit(int checkDigit_) { checkDigit = checkDigit_; }

  std::string getInputData() const { return inputData; }
  void setInputData(std::string inputData_) { inputData = inputData_; }

  std::string getHumanReadableText() const { return human_readable_text; }

private:

  std::string inputData;
  std::string human_readable_text;
  int checkDigit;
  std::string result;
  std::string CODE39MAP[43]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","-","."," ","$","/","+","%"};

  int getCode39Value(char inputchar) {
	int RVal=-1;
	int i=0;
	for (i=0;i<43;i++)
	{
		if (inputchar==CODE39MAP[i][0])
		{
			RVal=i;
		}
	}
	return RVal;
  }

  std::string generateCheckDigit(std::string data)
  {
	std::string checkDigit;
	int datalength=data.length();
	int sumValue=0;
	int x=0;
	for (x=0;x<datalength;x++)
	{
		sumValue=sumValue+getCode39Value(data[x]);
	}
	sumValue=sumValue % 43;
	return CODE39MAP[sumValue];
  }

  std::string filterInput(std::string data)
  {
	std::string result;
	int x=0;
	int y=0;
	for (x=0; x < data.length() && y < 255; x++)
	{
		if (getCode39Value(data[x]) != -1)
		{
			result=result+data[x];
			y++;	
		}
	}
	return result;
  }

};

// Binding code
EMSCRIPTEN_BINDINGS(connectcode_code39) {
  class_<Code39>("Code39")
    .constructor<>()
    .constructor<std::string, int>()
    .function("encode", &Code39::encode)
    .property("checkDigit", &Code39::getCheckDigit, &Code39::setCheckDigit)
    .property("inputData", &Code39::getInputData, &Code39::setInputData)
    .property("humanReadableText", &Code39::getHumanReadableText)
    ;
}
