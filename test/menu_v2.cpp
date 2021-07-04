//menu_v2.cpp

/* questions
	может отдать фиксированное количество символов под поля?
	связи имя(котнроллер) -> имя(прошивка)
	класс datalink какой вид map
	переписать все в .hpp, .cpp
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>


const std::string CONTROLLERS_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/controllers.txt";
const std::string FIRMWARES_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/firmwares.txt";
const std::string LINK_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/link.txt";

class Firmware{
protected:
	std::string name; 
	std::string path; //file path
	std::string description; //example: "firmware for the sensor dht11"
	int id; //toDO used to link

public:
	Firmware(std::string _name = "f_name",  std::string _path = "f_path", std::string _desc = "f_desc", int _id = 0): 
		name(_name), path(_path), description(_desc), id(_id){};

	void setName(std::string _name) {name = _name;};

	void setPath(std::string _path) {path = _path;};

	void setDesciption(std::string _desc) {description = _desc;};

	std::string getName() {return name;};

	std::string getPath() {return path;};

	std::string getDescription() {return description;};

	void show(){
		std::cout << "firmware:	name = \"" << name <<"\", path = \"" << path << "\", desc = \"" << description << "\"" << std::endl;
	}

	void writeToFile(std::string firmwares){
		std::ofstream out(firmwares);
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
	Firmware firmware;
	int id;	//toDO used to link

public:
	//Controller():	name("c_name"), ip("0.0.0.0"), description("c_desc"){};
	Controller(std::string _name = "c_name",  std::string _ip = "0.0.0.0", std::string _desc = "c_desc", int _id = 0): 
		name(_name), ip(_ip), description(_desc), id(_id){
		Firmware();
	};

	void setName(std::string _name) {name = _name;};

	void setIp(std::string _ip) {ip = _ip;};

	void setDesciption(std::string _desc) {description = _desc;};

	std::string getName() {return name;};

	std::string getIp() {return ip;};

	std::string getDescription() {return description;};

	void writeToFile(std::string controllers){
		std::ofstream out(controllers);
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
	std::string controllers;
	std::string firmwares;
	std::string link;
	std::vector<Controller> vController;
	std::vector<Firmware> vFirmware;
	std::map <std::string, std::string> data; //toDO std::map <Controller, Firmware> data;

public:
	DataLink(std::string _controllers = CONTROLLERS_FILE, std::string _firmwares = FIRMWARES_FILE, std::string _link = LINK_FILE):
		controllers(_controllers), firmwares(_firmwares), link(_link){};

	Controller readController(){
		std::ifstream file(controllers); 
		Controller controller;
		std::string name, ip, description;

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
				data.insert({controller.getName(),"none"});
			}
		}
		file.close();

		return controller;
	}

	Firmware readFirmware(){
		std::ifstream file(firmwares); 
		Firmware firmware;
		std::string name, path, description;

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

		return firmware;
	}

	void pairMap(){
		show_map();
		show_vController();
		show_vFirmware();
	}

	void show_vFirmware(){
		std::cout << "Vector firmware:" << std::endl;
		for (auto it : vFirmware){ 
			std:: cout << "\t" << it.getName() ;
		}
		std::cout << std::endl;
	}

	void show_vController(){
		std::cout << "Vector controller:" << std::endl;
		for (auto it : vController){ 
			std:: cout << "\t" << it.getName();
		}
		std::cout << std::endl;
	}

	void show_map(){
		std::cout << "Link data:" << std::endl;
		for (auto device : data) {
            std::cout << "\t" <<device.first << " -> " << device.second << std::endl;
    	}
	}

};


int main(){
	std::string f_name = "ota_ledflash", path = "/Users/bair/Documents/Arduino/NodeMCU/NodeMCU_ota_ledflash/NodeMCU_ota_ledflash.ino", 
		description = "none", c_name = "led", ip = "0.0.0.0";
	
	Firmware f1(f_name, path, description);
	Controller c1(c_name, ip, description); 
	
	c1.writeToFile(CONTROLLERS_FILE);
	c1.readFromFile(CONTROLLERS_FILE);

	std::cout << std::endl;
	f1.writeToFile(FIRMWARES_FILE);
	f1.readFromFile(FIRMWARES_FILE);

	std::cout << std::endl;
	DataLink link;
	link.readController();
	link.readFirmware();

	link.pairMap();

	return 0;
}