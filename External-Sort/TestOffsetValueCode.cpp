// g++ defs.cpp  OVC_Test.cpp OffsetValueCode.cpp DataRecord.cpp -o OVC_Test.exe

#include "DataRecord.h"

int main() {
    DataRecord rec1 = DataRecord(1234, 1234, 1234);
    DataRecord rec2 = DataRecord(5678, 5678, 5678);
    DataRecord rec3 = DataRecord(9876, 9886, 9876);

    bool rec1_is_smaller_than_2 = rec1.is_smaller_int(rec2);
    if (rec1_is_smaller_than_2) {
        std::cout<<"Data record 1 is smaller than record 2";
    } else {
        std::cout<<"Data record 1 is smaller than record 2";
    }

    bool rec3_is_smaller_than_2 = rec3.is_smaller_int(rec2);
    if (rec3_is_smaller_than_2) {
        std::cout<<"Data record 3 is smaller than record 2";
    } else {
        std::cout<<"Data record 3 is larger than record 2";
    }

    rec2.update_or_create_ov_code(rec1);
    rec3.update_or_create_ov_code(rec2);

    rec2.print();
    rec3.print();
    return 0;
}
