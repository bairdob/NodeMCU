//menu_v2.cpp

/* questions
	может отдать фиксированное количество символов под поля?
	связи имя(котнроллер) -> имя(прошивка)
	класс datalink какой вид map
	переписать все в .hpp, .cpp
	может векторы контроллеры и прошифки хранить сразу в DataLink
	прочитать про стайлгайд 
	логика после ввода контроллера/прошивки сразу запись в файл, затем заполняем файл со связями
	класс сонтроллер содержит прошивку?
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ncurses.h>
//#include <Python.h>


const std::string CONTROLLERS_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/controllers.txt";
const std::string FIRMWARES_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/firmwares.txt";
const std::string LINK_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/link.txt";

class Firmware{
protected:
	std::string name; 
	std::string path; //file path
	std::string description; //example: "firmware for the sensor dht11"

public:
	Firmware(std::string _name = "f_name",  std::string _path = "f_path", std::string _desc = "f_desc"): 
		name(_name), path(_path), description(_desc){};

	void setName(std::string _name) {name = _name;};

	void setPath(std::string _path) {path = _path;};

	void setDesciption(std::string _desc) {description = _desc;};

	std::string getName() {return name;};

	std::string getPath() {return path;};

	std::string getDescription() {return description;};

	void show(){
		std::cout << "firmware:	name = \"" << name <<"\", path = \"" << path << "\", desc = \"" << description << "\"" << std::endl;
	}

	void writeToFile(){
		std::ofstream out(FIRMWARES_FILE, std::ofstream::app);

		if (out.is_open()){
	    	out << name << ";" << path << ";" << description << std::endl;
		}

		out.close();
	};

	void readFromFile(std::string firmwares){
		std::string line;
		std::ifstream in(firmwares); 

	    if (in.is_open()){
	        while (getline(in, line)){
	            std::cout << line << std::endl;
	        }
	    }

	    in.close(); 		
	}
};


class Controller{
private:
	std::string name;
	std::string ip;
	std::string description;
	Firmware firmware; //toDO

public:
	Controller(std::string _name = "c_name",  std::string _ip = "0.0.0.0", std::string _desc = "c_desc"): 
		name(_name), ip(_ip), description(_desc){
		Firmware();
	};

	void setName(std::string _name) {name = _name;};

	void setIp(std::string _ip) {ip = _ip;};

	void setDesciption(std::string _desc) {description = _desc;};

	std::string getName() {return name;};

	std::string getIp() {return ip;};

	std::string getDescription() {return description;};

	void writeToFile(){
		std::ofstream out(CONTROLLERS_FILE, std::ofstream::app);

		if (out.is_open()){
	    	out << name << ";" << ip << ";" << description << std::endl;
		}

		out.close();
	};

	void readFromFile(std::string controllers){
		std::string line;
		std::ifstream in(controllers); 

	    if (in.is_open()){
	        while (getline(in, line)){
	            std::cout << line << std::endl;
	        }
	    }

	    in.close(); 		
	}

	void show(){
		std::cout << "controller:	name = \"" << name <<"\", ip = \"" << ip << "\", desc = \"" << description << "\"" << std::endl;
		firmware.show();
	}
};


class DataLink {
private:
	std::vector<Controller> vController;
	std::vector<Firmware> vFirmware;
	std::map <std::string, std::string> data; //toDO std::map <Controller, Firmware> data;

public:
	DataLink(){};

	void readController(){
		std::ifstream file(CONTROLLERS_FILE); 
		Controller controller;
		std::string name, ip, description;
		vController.clear();

	    if (file.is_open()){    
	        std::string line;

			while (getline(file, line)){
				std::istringstream iss(line); 
				std::string token;
				int i = 0;

				while (getline(iss, token, ';')) {
					if (i == 0) name = token; controller.setName(name); //toDO rewrite
					if (i == 1) ip = token;	controller.setIp(ip);
					if (i == 2) description = token; controller.setDesciption(description);	
					i++;
				}
					 
				// std::cout << "Data:" << std::endl;
				// std::cout << "\tName: " << controller.getName() << std::endl;
				// std::cout << "\tIp: " << controller.getIp() << std::endl;
				// std::cout << "\tDescription: " << controller.getDescription()<< std::endl;
				vController.push_back(controller);
			}
		}
		file.close();
	}

	void readFirmware(){
		std::ifstream file(FIRMWARES_FILE); 
		Firmware firmware;
		std::string name, path, description;
		vFirmware.clear();

	    if (file.is_open()){    
	        std::string line;

			while (getline(file, line)){
				std::istringstream iss(line); 
				std::string token;
				int i = 0;

				while (getline(iss, token, ';')) {
					if (i == 0) name = token; firmware.setName(name); //toDO rewrite
					if (i == 1) path = token;	firmware.setPath(path);
					if (i == 2) description = token; firmware.setDesciption(description);	
					i++;
				}
					 
				// std::cout << "Data:" << std::endl;
				// std::cout << "\tName: " << firmware.getName() << std::endl;
				// std::cout << "\tIp: " << firmware.getIp() << std::endl;
				// std::cout << "\tDescription: " << firmware.getDescription()<< std::endl;
				vFirmware.push_back(firmware);
			}
		}
		file.close();

	}

	void readLink(){
		std::ifstream file(LINK_FILE); 
		std::string controller_name, firmware_name;


	    if (file.is_open()){    
	        std::string line;

			while (getline(file, line)){
				std::istringstream iss(line); 
				std::string token;
				int i = 0;

				while (getline(iss, token, ';')) {
					if (i == 0) controller_name = token; 
					if (i == 1) firmware_name = token;
					
					i++;
				}
				data.insert ({controller_name, firmware_name});
				// std::cout << "Map:" << std::endl;
				// std::cout << "\tC_Name: " << controller_name << std::endl;
				// std::cout << "\tF_name: " << firmware_name << std::endl;				
			}
		}
		file.close();

	}


	void update(){
		readController();
		readFirmware();
		readLink();
	}

	void pairMap(){
		update();
		show_map();
		show_vController();
		show_vFirmware();
		std::string c_name, f_name;

		std::map<std::string, std::string>::iterator it;
		std::cout << "Enter controller name: " << std::endl;
		std::cin >> c_name;
		// it = data.find(c_name);
	 //    if(it == data.end())
	 //        std::cout << "Key-value pair not present in map" ;
	 //    else{
			std::cout << "Enter firmware name: " << std::endl;
			std::cin >> f_name;
			// data.erase(it);
			data.insert ({c_name, f_name});
	    // }
	    writeToFile();

	}

	void show_vFirmware(){
		update();
		std::cout << "Vector firmware:" << std::endl;
		for (auto it : vFirmware){ 
			std:: cout << '\t' << it.getName();
		}
		std::cout << std::endl;
	}

	void show_vController(){
		update();
		std::cout << "Vector controller:" << std::endl;
		for (auto it : vController){ 
			std:: cout << '\t' << it.getName();
		}
		std::cout << std::endl;
	}

	void show_map(){
		update();
		std::cout << "Link data:" << std::endl;
		for (auto device : data) {
            std::cout << '\t' << device.first << " -> " << device.second << std::endl;
    	}
	}

	void writeToFile(){
		std::ofstream out(LINK_FILE, std::ofstream::trunc);
		if (out.is_open()){
			for (auto device : data) {
            	out << device.first << ";" << device.second << std::endl;
    		}
		}
		out.close();
	};

	std::string findIpByName(std::string name){
		std::string ip;
		for (auto it: vController){
			if (it.getName() == name) ip = it.getIp();
		}
		return ip;
	}

	std::string findPathByName(std::string name){
		std::string path;
		for (auto it: vFirmware){
			if (it.getName() == name) path = it.getPath();
		}
		return path;
	}

	std::string getMapValue(std::string key){
		std::string value = data.at(key);
		return value;
	}

	std::string otaParameters(std::string name){
		return findIpByName(name) + " " + findPathByName(getMapValue(name)) ;		
	}

};

/*
menu interface
*/

void print_menu(){
	std::cout << "Выберите действие:" << std::endl;
	std::cout << "1 - Загрузить прошивки" << std::endl;
	std::cout << "2 - Добавить новый контроллер" << std::endl;
	std::cout << "3 - Удалить/изменить контроллер" << std::endl; 
	std::cout << "4 - Добавить прошивку" << std::endl;
	std::cout << "5 - Удалить/изменить прошивку" << std::endl;
	std::cout << "6 - Выход" << std::endl;
}

void addController(){	//("2 - Добавить новый контроллер")
	std::string name;
	std::cout << "Enter controller name: " << std::endl;
	std::cin >> name; 

	std::string ip;
	std::cout << "Enter controller IP: " << std::endl;
	std::cin >> ip; 

	std::string description;
	std::cout << "Enter controller description: " << std::endl;
	std::cin >> description; 

	Controller temp;
	temp.setName(name);
	temp.setIp(ip);
	temp.setDesciption(description);
	temp.writeToFile();

	print_menu();
}

void addFirmware(){	//("4 - Добавить новую прошивку")
	std::string name;
	std::cout << "Enter firmware name: " << std::endl;
	std::cin >> name; 

	std::string path;
	std::cout << "Enter firmware path: " << std::endl;
	std::cin >> path; 

	std::string description;
	std::cout << "Enter firmware description: " << std::endl;
	std::cin >> description; 

	Firmware temp;
	temp.setName(name);
	temp.setPath(path);
	temp.setDesciption(description);
	temp.writeToFile();

	print_menu();
}

void echoToTerminal(std::string key){
   	FILE* file;
    int argc;
    char const * argv[4];

    argc = 4;
    argv[0] = "espota.py";
    argv[1] = "-d";
    argv[2] = "-i";
    argv[3] = new char[key.length() + 1];

    Py_SetProgramName(argv[0]);
    Py_Initialize();
    PySys_SetArgv(argc, argv);
    file = fopen("espota.py","r");
    PyRun_SimpleFile(file, "espota.py");
    Py_Finalize();

    // char const *cmd = new char[key.length() + 1];
    // char buf[BUFSIZ];
    // FILE *ptr;

    // if ((ptr = popen(cmd, "r")) != NULL)
    //     while (fgets(buf, BUFSIZ, ptr) != NULL)
    //                 (void) printf("%s", buf);
    // (void) pclose(ptr);	
}

void loadFirmware(DataLink &link){ 
	std::string name;
	std::cout << "Enter controller name: " << std::endl;
	std::cin >> name; 
	std::string otaKey = link.otaParameters(name);
	echoToTerminal(otaKey);
	print_menu();
}; 

void changeFirmware(){ //toDO

}; 

int main(){
	DataLink link;
	// link.update();

	// link.show_map();
	// std::cout << link.otaParameters("cname2");
	system("clear");
	print_menu();

	int choice = 0;

	while (choice != 6){
		std::cin >> choice; 
		switch (choice) {	
	            case 1:	//("1 - Загрузить прошивки")
	            	system("clear");
	            	link.show_map();
	          		loadFirmware(link);
	                break;
	            case 2:	//("2 - Добавить новый контроллер")
	            	system("clear");
	            	link.show_vController();
	                addController();
	                break;
	            case 3: //("3 - Удалить/изменить контроллер")
	            	system("clear");
	            	link.pairMap();
	            	print_menu();
	                break;
	            case 4: //("4 - Добавить прошивку");
	            	system("clear");
	            	link.show_vFirmware();
	           		addFirmware();
	                break;
	            case 5: //("5 - Удалить/изменить прошивку");
	            	system("clear");
	           		changeFirmware();//toDO
	                break;
	            case 6:	//("6 - Выход");
	                break;
	            default:
	            	break;
	    }
    }


	return 0;
}