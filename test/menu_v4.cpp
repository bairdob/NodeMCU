//menu_v2.cpp

/* questions

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>


const std::string CONTROLLERS_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/controllers.txt";
const std::string FIRMWARES_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/firmwares.txt";
const std::string LINK_FILE= "/Users/bair/Documents/Arduino/NodeMCU/test/data/link.txt";
const int HASH_SIZE = 100; //maximum saved devices

int hashSumm(std::string str1, std::string str2, std::string str3){
	std::string word = str1 + str2 + str3;
	int sum = 0;

    for (int k = 0; k < word.length(); k++){
        sum = sum + int(word[k]);
    }
    if ((sum % HASH_SIZE) == (HASH_SIZE - 48)) return -1; //-1 for default controller constructor it means no firmware
    	else return  sum % HASH_SIZE;
}

class Firmware{
protected:
	int id;
	std::string name; 
	std::string path; //file path
	std::string description; //example: "firmware for the sensor dht11"

public:
	Firmware(std::string _name = "f_name",  std::string _path = "f_path", std::string _desc = "f_desc"): 
		name(_name), path(_path), description(_desc){
			id = hashSumm (name, path, description);
	};

	void setId(int _id) {id = _id;};

	void setId(std::string name,std::string path, std::string description) {id = hashSumm(name, path, description);};

	void setName(std::string _name) {name = _name;};

	void setPath(std::string _path) {path = _path;};

	void setDesciption(std::string _desc) {description = _desc;};

	int getId() {return id;};

	std::string getName() {return name;};

	std::string getPath() {return path;};

	std::string getDescription() {return description;};

	Firmware getFirmware() {return *this;};

	void const show(){
		//std::cout << "firmware:" << std::endl;
		std::cout << "\tid = \"" << id << "\", name = \"" << name <<"\", path = \"" << path << "\", desc = \"" << description << "\"" << std::endl;
	}

	void writeToFile(){
		std::ofstream out(FIRMWARES_FILE, std::ofstream::app);

		if (out.is_open()){
	    	out << id << ";" << name << ";" << path << ";" << description << std::endl;
		}

		out.close();
	};
};


class Controller : public Firmware{
private:
	std::string name;
	std::string ip;
	std::string description;
	Firmware firmware; //toDO

public:
	Controller(std::string _name = "c_name",  std::string _ip = "0.0.0.0", std::string _desc = "c_desc"): 
		name(_name), ip(_ip), description(_desc), firmware(){};

	void setName(std::string _name) {name = _name;};

	void setIp(std::string _ip) {ip = _ip;};

	void setDesciption(std::string _desc) {description = _desc;};

	void setFirmware(Firmware _firmware) {firmware = _firmware;};

	void setId(int _id) {firmware.setId(_id);};

	std::string getName() {return name;};

	std::string getIp() {return ip;};

	std::string getId() {return std::to_string(firmware.getId() );};

	std::string getDescription() {return description;};

	std::string getPath() {return firmware.getPath();};

	void writeToFile(){
		std::ofstream out(CONTROLLERS_FILE, std::ofstream::app);

		if (out.is_open()){
	    	out  << firmware.getId() << ";"<< name << ";" << ip << ";" << description << std::endl;
		}

		out.close();
	};

	void const show(){
		//std::cout << "controller:" << std::endl;
		std::cout << "\tname = \"" << name << "\", f_id = \"" << firmware.getId() <<"\", ip = \"" << ip << "\", desc = \"" << description << "\"" << std::endl;
	}
};

class DataBase{
public:
	std::vector<Firmware> vFirmware;
	std::vector<Controller> vController;

	DataBase(){
		readControllerFile();
		readFirmwareFile();
	}

	void readFirmwareFile(){
		std::ifstream file(FIRMWARES_FILE); 
		Firmware firmware;
		std::string name, path, description;
		int id;
		vFirmware.clear();

	    if (file.is_open()){    
	        std::string line;

			while (getline(file, line)){
				std::istringstream iss(line); 
				std::string token;
				int i = 0;

				while (getline(iss, token, ';')) {
					if (i == 0) id = std::stoi(token); firmware.setId(id); //toDO rewrite
					if (i == 1) name = token; firmware.setName(name); 
					if (i == 2) path = token;	firmware.setPath(path);
					if (i == 3) description = token; firmware.setDesciption(description);	
					i++;
				}
				vFirmware.push_back(firmware);
			}
		}
		file.close();
	}

	void readControllerFile(){
		std::ifstream file(CONTROLLERS_FILE); 
		Controller controller;
		std::string name, ip, description;
		int id;
		vController.clear();

	    if (file.is_open()){    
	        std::string line;

			while (getline(file, line)){
				std::istringstream iss(line); 
				std::string token;
				int i = 0;

				while (getline(iss, token, ';')) {
					if (i == 0) id = std::stoi(token); controller.setId(id); //toDO rewrite
					if (i == 1) name = token; controller.setName(name); 
					if (i == 2) ip = token;	controller.setIp(ip);
					if (i == 3) description = token; controller.setDesciption(description);	
					i++;
				}
				vController.push_back(controller);
			}
		}
		file.close();
	}

	void show_vFirmware (){
		std::cout << "Firmwares:" << std::endl;
		for (auto &it : vFirmware) {
			it.show();
		}
	}

	void show_vController (){
		std::cout << "Controllers:" << std::endl;
		for (auto &it : vController) {
			it.show();
		}
	}

	std::string getPythonParameters(std::string str){
		for (auto it : vController){
			if (it.getName() == str){
				for (auto &fit : vFirmware){
					if (std::stoi(it.getId()) == fit.getId()) {
						return "python espota.py -d  -i " + it.getIp() + " -f " + fit.getPath();
					}
				}
			}
		} 
		return "none";
	}

	void changeController(std::string name, std::string id){
		for (auto &it : vController) {
			if (name == it.getName()) {
				it.setId(std::stoi(id));
				it.show();
				for (auto &fit : vFirmware){
					if (std::stoi(id) == fit.getId()) {
						it.setFirmware(fit.getFirmware()); 
					}
				}
			}
		}
	}

	void writeToFileVController(){
		std::ofstream out(CONTROLLERS_FILE, std::ofstream::trunc);

		if (out.is_open()){
			for (auto &it:vController)
	    	out  << it.getId() << ";"<< it.getName() << ";" << it.getIp() << ";" << it.getDescription() << std::endl;
		}

		out.close();
	}

	void writeToFileVFirmware(){
		std::ofstream out(FIRMWARES_FILE, std::ofstream::trunc);

		if (out.is_open()){
			for (auto &it:vFirmware)
	    	out  << it.getId() << ";"<< it.getName() << ";" << it.getPath() << ";" << it.getDescription() << std::endl;
		}

		out.close();
	}
};

void print_menu(){
	std::cout << "Выберите действие:" << std::endl;
	std::cout << "1 - Загрузить прошивки" << std::endl;
	std::cout << "2 - Добавить новый контроллер" << std::endl;
	std::cout << "3 - Удалить/изменить контроллер" << std::endl; 
	std::cout << "4 - Добавить прошивку" << std::endl;
	std::cout << "5 - Удалить/изменить прошивку" << std::endl;
	std::cout << "6 - Выход" << std::endl;
}

void addController(DataBase db){	//("2 - Добавить новый контроллер")
	system("clear");
	std::cout << "2 - Добавить новый контроллер" << std::endl;
	db.show_vController();
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

void changeController(DataBase db){
	system("clear");
	std::cout << "3 - Удалить/изменить контроллер" << std::endl; 
	db.readControllerFile();
	db.show_vController();
	std::string name;
	std::cout << "Enter controller name: " << std::endl;
	std::cin >> name; 

	db.readFirmwareFile();
	db.show_vFirmware();
	std::string firmware;
	std::cout << "Enter firmware id: " << std::endl;
	std::cin >> firmware; 

	db.changeController(name, firmware);
	db.writeToFileVController();
}

void addFirmware(){	//("4 - Добавить новую прошивку")
	system("clear");
	std::cout << "4 - Добавить прошивку" << std::endl;
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
	temp.setId(name, path, description);
	temp.writeToFile();

	print_menu();
}


void changeFirmware(){ //toDO

}; 

void loadController(DataBase db){
	system("clear");
	std::cout << "1 - Загрузить прошивки" << std::endl;
	db.readControllerFile();
	db.show_vController();
	std::string name;
	std::cout << "Enter controller name: " << std::endl;
	std::cin >> name; 
	std::string cmd = db.getPythonParameters(name);
	//std::cout << db.getPythonParameters(name);
	system(cmd.c_str()); 
}


int main(){
	DataBase db;
	system("clear");
	print_menu();

	int choice = 0;

	while (choice != 6){
		std::cin >> choice; 
		switch (choice) {	
	            case 1:	//("1 - Загрузить прошивки")
	            	loadController(db);
	                break;

	            case 2:	//("2 - Добавить новый контроллер")
	            	
	            	addController(db);
	                break;

	            case 3: //("3 - Удалить/изменить контроллер")
	            	
	            	changeController(db);
	                break;

	            case 4: //("4 - Добавить прошивку");
	            	db.show_vFirmware();
	            	addFirmware();
	                break;

	            case 5: //("5 - Удалить/изменить прошивку");
	            	system("clear");
	            	db.show_vFirmware();
	                break;

	            case 6:	//("6 - Выход");
	                break;

	            default:
	            	break;
	    }
    }

	return 0;
}