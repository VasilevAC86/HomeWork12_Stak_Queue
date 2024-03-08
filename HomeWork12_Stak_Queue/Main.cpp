# include <iostream>
#include<fstream>
#include<string>
#include<stack>
#include<queue>
#include<random>

/// <summary>
/// Функция для смещения очереди queue на 1 позицию
/// </summary>
/// <param name="qwe">Ссылка на очередь</param>
void change_queue(std::queue<std::string>& qwe) {
	std::string tmp; // Переменная для временного хранения ФИО	
	tmp = qwe.front();
	qwe.pop();
	qwe.push(tmp);
}

int main() {	
	std::string path = "People.txt"; // Относительный путь к исходному файлу "People.txt"
	std::string path_result = "Result.txt"; // Относительный путь к итоговому файлу "Result.txt"
	std::ifstream in; // Объект для чтения информации из файла
	in.open(path, std::ios::in); // Открываем файл для чтения
	if (in.is_open()) { // Если файл успешно открыт для чтения

	// ================================== Шаг 1 - Считывание данных из файла в очередь ========================================

		std::cout << "File \033[93m" << path << "\033[0m is open for reading.\n\n";
	}
	else { // Если файл не получилось открыть
		std::cout << "\033[94mError opening file!\033[0m\n";
		throw -1; // Кидаем исключение
	}
	std::stack<std::string> people_stack; // Создаём адптер контейнера типа FILO (последний добавленный элемент типа string будет извлечён первым)
	std::queue<std::string> people_queue; // Создаём адптер контейнера типа FIFO (последний добавленный элемент типа string будет извлечён последним)		
	int length = 0; // Переменная для хранения максимальной длины ФИО (для последующего форматированного вывода данных в консоль)
	char my_choice; // Переменная для хранения выбора пользователя	
	std::cout << "\033[93mDo you want to output data from a file in RANDOM order?\033[93m\nPress \033[91m'1'\033[0m to yes \033[4mor\033[0m press \033[91many other key\033[0m to no\nYour choice -> ";
	std::cin >> my_choice;
	{ // Создаём локальную область видимости для того, чтобы переменная str создавалась и уничтожалась только один раз
		std::string str; // Переменная для построчного считывания ФИО из файла			
		while (std::getline(in, str)) { // Пока в строке файла есть что считывать, заполняем очередь считанной в строке файла информацией
			people_queue.push(str);
			// Если пользователь не выбрал рандомное заполнение адаптеров коллекций, то заполняем стек считанной из файла строкой
			if (my_choice != '1')  
				people_stack.push(str);
			if (length < str.length())
				length = str.length(); // Определяем длину самой длиной ФИО в кол-ве символов			
		}
	}
	in.close(); // После считывания всей информации из файла, закрываем его

	// =================================== Шаг 2 - Перемешивание данных в адаптерах коллекций по выбору пользователя ==================================
		
	// Если пользователь выбрал рандомное заполнение адаптеров коллекций, то рандомно смещаем очередь, стэк заполняем очередью, рандомно смещённой ещё раз
	if (my_choice == '1') { 
		//std::stack<std::string> tmp_people_stack; // Временный стэк
		std::queue<std::string> tmp_queue; // Временная очередь
		std::random_device rd; // Создаём зерно генерации истинно случайных чисел
		std::mt19937 provide(rd()); // Создаём генератор псевдослучайных чисел			
		while (!people_queue.empty()) {	// Пока в очереди что-то есть
			// Создаём доставщика случайного числа от 1 до текущего размера адаптера коллекций				
			std::uniform_int_distribution<int> distributor(1, people_queue.size()); 
			for (int i = 0; i < distributor(provide); ++i) 
				change_queue(people_queue);				
			tmp_queue.push(people_queue.front()); // Во временную очередь записываем первый элемент смещённой случайным образом исходной очереди
			people_queue.pop(); // Удаляем первый элемент смещённой случайным образом исходной очереди, уменьшая её на 1
		}
		people_queue = tmp_queue; // Перезаписываем исходныю очередь очередью, сформированной рандомным порядком		
		// Заполнение стэка рандомным образом реализуем за счёт очередного рандомного смещения очереди.	Стэк заполняем значениями рандомно перестроенной очереди
		while (!tmp_queue.empty()) {	// Пока в очереди что-то есть
			std::uniform_int_distribution<int> distributor(1, tmp_queue.size());
			for (int i = 0; i < distributor(provide); ++i) 
				change_queue(tmp_queue);				
			people_stack.push(tmp_queue.front());
			tmp_queue.pop();
		}
	}	

	// ========== Шаг 3 - Вывод в консоль и титоговый файл двух списков имём в аккуратных столбцах в соответствии с выбором пользователя =====================

	std::ofstream out; // Создаём объект для записи в файл
	out.open(path_result, std::ios::out); // Открываем файл для записи (создаём файл, если его ещё нет)
	if (out.is_open()) // Если файл успешно открыт для записи
		std::cout << "\nFile \033[93m" << path_result << "\033[0m is open for rewriting.\n";
	else { // Если файл для записи открыть не удалось
		std::cout << "\n\033[94mError opening file!\033[0m\n";
		throw - 1; // Кидаем исключение
	}
	int size = people_queue.size(); // Кол-во людей в файле	= размеру очереди = размеру стэка
	std::string name_first_column = "The first column ";
	if (length < name_first_column.length()) { // Если длина самого длинного имени маньше длины наименования столбца, то
		std::cout << "\n\033[92m" << name_first_column << "\033[0m\033[93m|\033[0m\033[92m The second column \033[0m";
		out << "The first column | The second column " << '\n';
		length = name_first_column.length();
	}
	else { // Если длина столбца меньше длины самого длинного имени, то увеличиваем длину столбца пробелами
		std::cout << "\n\033[92m" << name_first_column << "\033[0m";
		out << "The first column ";
		for (int i = 0; i < length - name_first_column.length(); ++i) {
			std::cout << ' '; // Дописываем в заголовке таблицы пробелы так, чтобы длина заголовка не была меньше самомого длинного ФИО
			out << ' ';
		}
		std::cout << "\033[0m\033[93m|\033[0m\033[92m The second column \033[0m";
		out << " | The second column " << '\n';
	}
	std::cout << std::endl;
	for (int i = 0; i < size; ++i) {
		if (people_queue.front() == people_stack.top()) // Если имена одинаковы, то смещаем текущую очередь на единицу
			change_queue(people_queue);					
		std::cout << people_stack.top(); // выводим в консоль ФИО из стэка (последний добавленный элемент в стэк)
		out << people_stack.top();
		if (people_stack.top().length() < length) // Если длина ФИО текущего человека меньше максимальной, то				
			for (int j = 0; j < length - people_stack.top().length(); ++j) {
				std::cout << ' '; // пишем пробелы до следующего столбца	
				out << ' ';
			}
		std::cout << "\033[93m|\033[0m ";
		out << " | ";
		people_stack.pop(); // Удаляем последний элемент из стэка (последний добавленный в стэк элемент)
		std::cout << people_queue.front(); // Выводим в консоль ФИО из очереди (первый добавленный элемент в очередь)
		out << people_queue.front() << '\n'; // Записываем последнюю строчку в файл и переносим курсор в файле на новую строку 
		people_queue.pop(); // Удаляем первый элемент из очереди (первый добавленный в очередь элемент)
		std::cout << std::endl;		
	}
	std::cout << "\nThe data has been successfully entered into file \033[92m" << path_result << "\033[0m\n";
	out.close(); // Закрываем перезаписанный файл
	
	return 0;
}


