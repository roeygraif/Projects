#include <iostream>
#include "utils.h"
#include "examDetails.h"

using std::cout;
using std::endl;
using std::string;
using std::ostream;


namespace mtm {


    ExamDetails::ExamDetails(int course_num, int month, int day, double hour, int exam_length,
                             const string &zoom_link) {

        if(course_num < 0){
            throw InvalidArgsException();
        }
        this->course_num = course_num;

        if (!isValidMonth(month)) {
            throw InvalidDateException();
        }
        this->month = month;

        if (!isValidDay(day)) {
            throw InvalidDateException();
        }
        this->day = day;

        if (!isValidHour(hour)) {
            throw InvalidTimeException();
        }
        this->hour = hour;

        if (!isValidLength(exam_length)) {
            throw InvalidDateException();
        }
        this->exam_length = exam_length;

        this->zoom_link = std::string(zoom_link);
    }


    ExamDetails::~ExamDetails() = default;


    ExamDetails::ExamDetails(const ExamDetails &exam_details) { //copy constructor
        this->course_num = exam_details.course_num;
        this->month = exam_details.month;
        this->day = exam_details.day;
        this->hour = exam_details.hour;
        this->exam_length = exam_details.exam_length;
        this->zoom_link = exam_details.zoom_link;
    }


    ExamDetails &ExamDetails::operator=(const ExamDetails &exam_details) {
        this->course_num = exam_details.course_num;
        this->month = exam_details.month;
        this->day = exam_details.day;
        this->hour = exam_details.hour;
        this->exam_length = exam_details.exam_length;
        this->zoom_link = exam_details.zoom_link;
        return *this;
    }


    string ExamDetails::getLink() const{
        return this->zoom_link;
    }

    void ExamDetails::setLink(string new_link) {
        this->zoom_link = new_link;
    }


    int ExamDetails::operator-(const ExamDetails &exam_details) const {
        int days_exam1 = this->month * 30 + this->day;
        int days_exam2 = exam_details.month * 30 + exam_details.day;
        return days_exam1 - days_exam2;
    }


    bool ExamDetails::operator<(const ExamDetails &exam_details) const {
        if (this->month < exam_details.month)
            return true;
        if (this->month == exam_details.month) {
            if (this->day < exam_details.day)
                return true;
            if (this->day == exam_details.day) {
                if (this->hour < exam_details.hour)
                    return true;
            }
        }
        return false;
    }


    ostream &operator<<(ostream &os, const ExamDetails &exam_details) {
        os << "Course Number: " << exam_details.course_num << endl;
        os << "Time: " << exam_details.day << "." << exam_details.month << " at "
           << (int)(exam_details.hour) << (isRoundNumber(exam_details.hour) ? ":00" : ":30") << endl;
        os << "Duration: " << exam_details.exam_length << ":00" << endl;
        os << "Zoom Link: " << exam_details.zoom_link << endl;
        return os;
    }


    ExamDetails ExamDetails::makeMatamExam() {
        ExamDetails mtm_exam(234124, 7, 28, 13, 3, string("https://tinyurl.com/59hzps6m"));
        return mtm_exam;
    }

}