//menu.cpp
/*questions
	пункт меню 3 оставить только удалить??? логика
	пункт меню 1 логика загрузка по имени и загрузить все
	как получать IP, через вывод arduino, записывать руками или выгружать в облако (задание статических ip и имени)
	сразу рефакторить в класс
*/
#include <iostream>
#include <string>
#include <map>


void printText(std::string str){
	std::cout << str << std::endl;
} 

void print_menu(){
	printText("Выберите действие:");
	printText("1 - Загрузить прошивки");
	printText("2 - Добавить новый контроллер");
	printText("3 - Удалить/изменить контроллер"); //toDO спросить оставить только удалить??? логика
	printText("4 - Добавить прошивку");
	printText("5 - Удалить/изменить прошивку");
	printText("6 - Выход");
}

void printMapData(const std::map <std::string, std::map <std::string, std::string> > &controllers){
    std::cout << "Number of controllers: " << controllers.size() << std::endl;
    printText("Name : firmware -> IP");
    for (auto device : controllers) {
        for (auto firmware : device.second)
            std::cout << device.first << " : " << firmware.first << " -> " << firmware.second << std::endl;
    }
};

void loadFirmware(std::map <std::string, std::map <std::string, std::string>> &controllers){	//("1 - Загрузить прошивки")
	system("clear");
	printMapData(controllers);
	//toDO load buttons, load_all
	print_menu();
};

std::string getControllerIp(){ //toDO getinfo from ...
	return "0.0.0.0";
}

void addController(std::map <std::string, std::map <std::string, std::string>> &controllers){	//("2 - Добавить новый контроллер")
	system("clear");

	std::string name;
	printText("Enter controller name: "); 
	std::cin >> name; 

	controllers.insert (make_pair (name, std::map <std::string, std::string>()));
	controllers[name].insert ({"none", "none"});

	print_menu();
}


void printControllers(std::map <std::string, std::map <std::string, std::string>> &controllers){ 
	for (auto device : controllers) {
        std::cout << device.first << std::endl;
    }
};

void changeController(std::map <std::string, std::map <std::string, std::string>> &controllers){ //("3 - Удалить/изменить контроллер")
	system("clear");

	printText("Added controllers by name: ");
	printControllers(controllers);

	std::string name;
	printText("Enter controller name: ");
	std::cin >> name;

	std::map <std::string, std::map <std::string, std::string>>::iterator it;
	it = controllers.find(name);
	if(it == controllers.end())
        std::cout << "Key-value pair not present in map" ;
    else{
		controllers.erase(it);
    }

    print_menu();
};

void changeFirmwareByName(std::string name,std::map <std::string, std::map <std::string, std::string>> &controllers){
	std::string path, IP;

	std::map <std::string, std::map <std::string, std::string>>::iterator it;
	it = controllers.find(name);
    if(it == controllers.end())
        std::cout << "Key-value pair not present in map" ;
    else{
        //std::cout << "Key-value pair present : " << it->first << std::endl;
     	auto s = it->second;
        printText("Enter firmware path: ");
		std::cin >> path;
		printText("Enter controller IP: ");
		std::cin >> IP; // toDO or getControllerIp()
		controllers.erase(it);
		controllers[name].insert ({path, IP});
    }
}

void addFirmware(std::map <std::string, std::map <std::string, std::string>> &controllers){	//("4 - Добавить прошивку");
	system("clear");

	printText("Added controllers by name: ");
	printControllers(controllers);
	std::string name;

	printText("Enter controller name: ");
	std::cin >> name;
	changeFirmwareByName(name, controllers);

	print_menu();
};

void changeFirmware(std::map <std::string, std::map <std::string, std::string>> &controllers){	//("5 - Удалить/изменить прошивку"); 
	system("clear");
	
	printText("Added controllers by name: ");
	printControllers(controllers);
	std::string name;

	printText("Enter controller name: ");
	std::cin >> name;
	changeFirmwareByName(name, controllers);

	print_menu();
};

int main(){
	std::map <std::string, std::map <std::string, std::string> > controllers;

	print_menu();

	int choice = 0;
	while (choice != 6){
		std::cin >> choice; //toDO incorrect input
		switch (choice) {	
	            case 1:	//("1 - Загрузить прошивки")
	          		loadFirmware(controllers);      
	                break;
	            case 2:	//("2 - Добавить новый контроллер")
	                addController(controllers);
	                break;
	            case 3: //("3 - Удалить/изменить контроллер")
	            	changeController(controllers);
	                break;
	            case 4: //("4 - Добавить прошивку");
	           		addFirmware(controllers);
	                break;
	            case 5: //("5 - Удалить/изменить прошивку");
	           		changeFirmware(controllers);
	                break;
	            case 6:	//("6 - Выход");
	                break;
	            default:
	            	break;
	    }
    }

	return 0;
}