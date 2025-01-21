#ifndef EXAM_DETAILS_H
#define EXAM_DETAILS_H

#include <iostream>

namespace mtm {
    class ExamDetails {
        private:
            int course_num;
            int month;
            int day;
            double hour;
            int exam_length;
            std::string zoom_link;


        public:
            class InvalidDateException : public std::exception {};

            class InvalidArgsException : public std::exception {};

            class InvalidTimeException : public std::exception {};

            ExamDetails(int course_num, int month, int day, double hour, int exam_length,
                        const std::string &zoom_link = std::string());

            ~ExamDetails();

            ExamDetails(const ExamDetails &exam_details);

            ExamDetails &operator=(const ExamDetails &exam_details);

            std::string getLink() const;

            void setLink(std::string new_link);

            int operator-(const ExamDetails &exam_details) const;

            bool operator<(const ExamDetails &exam_details) const;

            friend std::ostream &operator<<(std::ostream &os, const ExamDetails &exam_details);

            static ExamDetails makeMatamExam();

    };
}

#endif