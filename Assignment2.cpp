// PHYS 30762 Programming in C++
// Assignment 2

// Program to compute mean, standard deviation and standard
// error of the a set of courses. Data is read from file,
// user is asked for mode of sorting data and what year, if
// andy, should be examined.


#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<algorithm>

// Function to return the first digit of an integer
int first_digit(int number){
    int first_digit = number;
    while(first_digit >= 10){
    first_digit = first_digit/10;
    }
    return first_digit;
};

//  Creating a class "Course" to contain all required attributes of a course
class course{
    protected:
        std::string course_name;
        int course_number;
        double average_mark;;
        int year;
    public:
        course(std::string name, int number, double mark){
            course_name = name;
            course_number = number;
            average_mark = mark; 
            year = first_digit(number);
        };
        std::string get_course_name(){
            return course_name;
            };
        std::string get_full_course_title(){
            return "PHYS "+ std::to_string(course_number) + " " + course_name;
            };
        int get_course_number(){
            return course_number;
            };
        int get_course_year(){
            return year;
            };
        double get_average_mark(){
            return average_mark;
            };

};
// Function to read a string and replace excess white spaces with a single one
// so strings can be correctly parsed without issue
void replace_n_whitespacings(int n, std::string& input_string){
    int pos;
    std::string whitespace;
    for(int i{};i < n; i++){
        whitespace.append(" ");
    }
    while( ( pos = input_string.find(whitespace) )!=std::string::npos ){
        input_string.replace( pos, 2, " " );};
};

// Function that reads an input vector reference, and replaces it with the mean,
// error and mean standard error of the vector
void mean_error_of_double_vector(std::vector<double>& values){
    int N = values.size();
    double sum_of_values{};
    double value_mean_difference_squared{};
    for(int i{}; i < values.size(); i++){
        sum_of_values += values.at(i);
        };
    double mean{sum_of_values/N};
    
    while(!values.empty()){
        value_mean_difference_squared += pow(values.front() - mean , 2);
        values.erase(values.begin());
    };
    double standard_deviation{sqrt(value_mean_difference_squared/(N-1))};
    values.push_back(mean);
    values.push_back(standard_deviation);
    values.push_back(standard_deviation/sqrt(N));
};

// Functions to compare two course objects, and return which is first numerically,
// by course number, or first alphabetically, by course name
bool numeric_sorting(course i, course j){
    return (i.get_course_number() < j.get_course_number());
};

bool alphabetical_sorting(course i, course j){
    int order{i.get_course_name().compare(j.get_course_name())};
    bool i_before_j;
    if(order <= 0){i_before_j=true;}
    else{i_before_j=false;};
    return i_before_j;
};    

void sort_vector(std::vector<course>& courses, bool numeric){
    if(numeric){
        std::sort(courses.begin(), courses.end(), numeric_sorting);
    }
    else{
        std::sort(courses.begin(), courses.end(), alphabetical_sorting);
    };
};

    
int main()
    {
    // Declaring vectors of course objects, to be used to sort input courses by year
    std::string data_file;
    std::string user_input;
    bool numeric_sorting;
    bool valid_input;
    int i;

    // Declaring vectors of course objects, to be used to sort input courses by year
    int number_courses;
    std::vector<course> first_year_courses;
    std::vector<course> second_year_courses;
    std::vector<course> third_year_courses;
    std::vector<course> fourth_year_courses;
    // Ask user to enter filename
    std::cout << "Enter data filename: ";
    std::cin >> data_file;
    // Open file and check if successful, else resarting code
    std::fstream course_stream(data_file);
    if(!course_stream.good()){
        std::cerr << "Error: That file could not be opened.\n";
        main();
    };
    // Read data from file
    std::string course_details;
    i=0;
    do{
        std::string line;
        std::string delim{" "};
        int pos;        
        std::vector<std::string>words;
        double mark{};
        int course_number{};
        std::string course_name{};
        // Formatting line of input data to be read, and splitting into individual words        
        // according to delimiter 
        std::getline(course_stream, line);
        replace_n_whitespacings(2, line);

        while((pos=line.find(delim)) != std::string::npos){
            words.push_back(line.substr(0,pos));
            line.std::string::erase(0, pos+delim.std::string::length());
        };
        words.push_back(line);
        // Interpretting words as arranged  by (Mark) (Course Number) (Course Name).
        // If a line cannot be read, an error is returned and user has choice to skip
        // the line and continue, or restart the program
        try{
        mark = std::stod(words.at(0));
        course_number = std::stoi(words.at(1));
        for(int i=2; i < words.size(); i++){
            course_name.append(words.at(i)+" ");
        };
        course course_token(course_name, course_number, mark);
        if(course_token.get_course_year() == 1){first_year_courses.push_back(course_token);}
        else if(course_token.get_course_year() == 2){second_year_courses.push_back(course_token);}
        else if(course_token.get_course_year() == 3){third_year_courses.push_back(course_token);}
        else{fourth_year_courses.push_back(course_token);};     
        i++;
        }
        catch(...){
            if(!course_stream.eof()) {
            i++;
            std::cerr << "\nUnable to read this data, line "<<i<< ":\n" << words.at(0) + " " + words.at(1) +" "+ line;
            std::cout << "\nPlease ensure each line is arranged (Mark) (Course Number) (Course Name), with whitespaces as delimiters.";
            std::cout << "\nWould you like to continue reading this file, ignoring this line, or restart? (ignore/restart)";}
            else if(course_stream.eof()){break;};
            do{
                std::cin.clear(); 
                std::cin.ignore(); 
                std::cin >> user_input;
                if(user_input == "ignore"){valid_input = true;}
                else if(user_input == "restart"){main();}
                else{
                    valid_input == false; std::cout << "\nSorry, that input wasn't recognised. Would you like to ignore or restart? (ignore/restart)";
                    }
            }
            while(!valid_input);
            }
        }
        while(!course_stream.eof());
    std::cout<<"\nDone! ";
    // Close file
    course_stream.close();
    // Print total number of lines successfully read in, compared to the total number
    long long unsigned int total_courses{first_year_courses.size()};
    total_courses += second_year_courses.size();
    total_courses += third_year_courses.size();
    total_courses += fourth_year_courses.size();
    std::cout << total_courses << " entries were read, out of a total of "<< i << ".";
    std::cout << "\nShould the courses be sorted by name or course code? (name/code)";
    valid_input = false;
    while(!valid_input){
        std::cin.clear();
        std::cin.ignore();
        std::cin >> user_input;
        if(user_input == "name"){
            numeric_sorting = false;
            valid_input = true;
        }
        else if(user_input == "code"){
            numeric_sorting = true;
            valid_input = true;
        }
        else{
            valid_input = false;
            std::cout << "\nSorry, that input wasn't recognised. Would you like to by name or course code? (name/code)";
        };
        }
    
    // Do-while loop to allow the user to see available all available courses, or by year,
    // as well as the mean, error and standard error in the mean of the selection
    bool continue_loop{true};
    do{
        std::cout << "\nWhat year group would you like to have course and mark details on?"
        "\n(1) First Year"
        "\n(2) Second Year"
        "\n(3) Third Year "
        "\n(4) Fourth Year"
        "\n(5) All years \n";
        valid_input = false;
        while(!valid_input){
                std::cin.clear();
                std::cin.ignore();
                std::cin >> user_input;
                if(user_input == "5"){
                    valid_input = true;
                    std::vector<course> combined_data;
                    std::vector<double> marks;
                    combined_data.reserve(total_courses);
                    combined_data.insert(combined_data.end(), first_year_courses.begin(), first_year_courses.end());
                    combined_data.insert(combined_data.end(), second_year_courses.begin(), second_year_courses.end());
                    combined_data.insert(combined_data.end(), third_year_courses.begin(), third_year_courses.end());
                    combined_data.insert(combined_data.end(), fourth_year_courses.begin(), fourth_year_courses.end());
                    sort_vector(combined_data, numeric_sorting);
                    std::cout << "\nCourse Title & Code                     Average Mark";
                    for(int i{};i<total_courses;i++){
                        marks.push_back(combined_data.at(i).get_average_mark());
                        std::cout << "\n"+ combined_data.at(i).get_full_course_title() + ":   " << marks.at(i);
                    };
                    mean_error_of_double_vector(marks);
                    std::cout <<  std::fixed << std::setprecision(1) << "\n\nThe average mark across all years is " << marks.at(0) << " +- " << marks.at(1) << " , standard error in mean = " << marks.at(2);
                    
                }
                else if(user_input == "1"){
                    valid_input = true;
                    std::vector<double> marks;
                    sort_vector(first_year_courses, numeric_sorting);
                    std::cout << "\nCourse Title & Code                     Average Mark";
                    for(int i{};i<first_year_courses.size();i++){
                        marks.push_back(first_year_courses.at(i).get_average_mark());
                        std::cout << "\n"+ first_year_courses.at(i).get_full_course_title() + ":   " << marks.at(i);
                    };
                    mean_error_of_double_vector(marks);
                    std::cout <<  std::fixed << std::setprecision(1) << "\n\nThe average mark for 1st year is " << marks.at(0) << " +- " << marks.at(1) << " , standard error in mean = " << marks.at(2);
                    
                }
                else if(user_input == "2"){
                    valid_input = true;
                    std::vector<double> marks;
                    sort_vector(second_year_courses, numeric_sorting);
                    std::cout << "\nCourse Title & Code                     Average Mark";
                    for(int i{};i<second_year_courses.size();i++){
                        marks.push_back(second_year_courses.at(i).get_average_mark());
                        std::cout << "\n"+ second_year_courses.at(i).get_full_course_title() + ":   " << marks.at(i);
                    };
                    mean_error_of_double_vector(marks);
                    std::cout <<  std::fixed << std::setprecision(1) << "\n\nThe average mark for 2nd year is " << marks.at(0) << " +- " << marks.at(1) << " , standard error in mean = " << marks.at(2);
                    
                }
                else if(user_input == "3"){
                    valid_input = true;
                    std::vector<double> marks;
                    sort_vector(third_year_courses, numeric_sorting);
                    std::cout << "\nCourse Title & Code                     Average Mark";
                    for(int i{};i<third_year_courses.size();i++){
                        marks.push_back(third_year_courses.at(i).get_average_mark());
                        std::cout << "\n"+ third_year_courses.at(i).get_full_course_title() + ":   " << marks.at(i);
                    };
                    mean_error_of_double_vector(marks);
                    std::cout <<  std::fixed << std::setprecision(1) << "\n\nThe average mark for 3rd year is " << marks.at(0) << " +- " << marks.at(1) << " , standard error in mean = " << marks.at(2);
                    
                }
                else if(user_input == "4"){
                    valid_input = true;
                    std::vector<double> marks;
                    sort_vector(fourth_year_courses, numeric_sorting);
                    std::cout << "\nCourse Title & Code                     Average Mark";
                    for(int i{};i<fourth_year_courses.size();i++){
                        marks.push_back(fourth_year_courses.at(i).get_average_mark());
                        std::cout << "\n"+ fourth_year_courses.at(i).get_full_course_title() + ":   " << marks.at(i);
                    };
                    mean_error_of_double_vector(marks);
                    std::cout <<  std::fixed << std::setprecision(1) << "\n The average mark for 4th year is " << marks.at(0) << " +- " << marks.at(1) << " , standard error in mean = " << marks.at(2);
                    
                }
                else{valid_input = false;
                std::cerr << "\nSorry, that input was not understood. Please input 1, 2 3, 4, or 5";};
            };
        std::cout << "\nWould you like to look at any more courses? (y/n)";
        valid_input = false;
        while(!valid_input){
            std::cin.clear();
            std::cin.ignore();
            std::cin >> user_input;            
            if(user_input == "y" || user_input == "n"){
                if(user_input == "y"){continue_loop = true, valid_input = true;}
                else if(user_input == "n"){continue_loop = false, valid_input = true;};
            }
            else{std::cerr << "\nSorry, that input was not understood. Please input y or n .";};
        }

    }
    while(continue_loop);
    return 0;
    }