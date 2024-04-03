#ifndef INPUT_H
#define INPUT_H

// Enum to represent the type of data being serialized
typedef enum {
    INTEGER,
    FLOAT,
    STRING
} DataType;

// Structure to represent serialized data along with metadata
typedef struct {
    DataType type;
    union {
        int integerData;
        float floatData;
        char stringData[50];
    } data;
} SerializedData;

#endif // INPUT_H