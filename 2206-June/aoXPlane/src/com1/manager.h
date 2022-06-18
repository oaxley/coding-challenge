// vim: set ft=ino
// 
// This source file is subject to the Apache License 2.0
// that is bundled with this package in the file LICENSE.txt.
// It is also available through the Internet at this address:
// https://opensource.org/licenses/Apache-2.0
// 
// @author	Sebastien LEGRAND
// @license	Apache License 2.0
// 
// @brief   Radio COM manager

#ifndef Manager_H
#define Manager_H

//----- globals
const int NUMBER_MAX = 136;
const int NUMBER_MIN = 118;

const int FRACTION_MAX = 95;
const int FRACTION_MIN = 0;


//----- class
class Manager {

    private:
        int number_;
        int fraction_;
        bool has_changed_;

    public:
        Manager() :
            number_(0), fraction_(0), has_changed_(false)
        { }

        ~Manager()
        { }

        // accessors
        int number() {
            has_changed_ = false;
            return number_;
        }

        int fraction() {
            has_changed_ = false;
            return fraction_;
        }

        void setValues(int value) {
            number_ = int(value / 100);
            fraction_ = value - (number_ * 100);
        }

        void incNumber() {
            has_changed_ = true;
            number_ = number_ + 1;
            if (number_ > NUMBER_MAX)
                number_ = NUMBER_MIN;
        }
        void decNumber() {
            has_changed_ = true;
            number_ = number_ - 1;
            if (number_ < NUMBER_MIN)
                number_ = NUMBER_MAX;
        }

        void incFraction() {
            has_changed_ = true;
            fraction_ = fraction_ + 5;
            if (fraction_ > FRACTION_MAX)
                fraction_ = FRACTION_MIN;
        }
        void decFraction() {
            has_changed_ = true;
            fraction_ = fraction_ - 5;
            if (fraction_ < FRACTION_MIN)
                fraction_ = FRACTION_MAX;
        }

        bool hasChanged() {
            return has_changed_;
        }

};

#endif // Manager_H