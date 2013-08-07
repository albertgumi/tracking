
/* Error Codes */
#define CL_SUCCESS                                  0
#define CL_DEVICE_NOT_FOUND                         -1
#define CL_DEVICE_NOT_AVAILABLE                     -2
#define CL_COMPILER_NOT_AVAILABLE                   -3
#define CL_MEM_OBJECT_ALLOCATION_FAILURE            -4
#define CL_OUT_OF_RESOURCES                         -5
#define CL_OUT_OF_HOST_MEMORY                       -6
#define CL_PROFILING_INFO_NOT_AVAILABLE             -7
#define CL_MEM_COPY_OVERLAP                         -8
#define CL_IMAGE_FORMAT_MISMATCH                    -9
#define CL_IMAGE_FORMAT_NOT_SUPPORTED               -10
#define CL_BUILD_PROGRAM_FAILURE                    -11
#define CL_MAP_FAILURE                              -12
#define CL_MISALIGNED_SUB_BUFFER_OFFSET             -13
#define CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST -14

#define CL_INVALID_VALUE                            -30
#define CL_INVALID_DEVICE_TYPE                      -31
#define CL_INVALID_PLATFORM                         -32
#define CL_INVALID_DEVICE                           -33
#define CL_INVALID_CONTEXT                          -34
#define CL_INVALID_QUEUE_PROPERTIES                 -35
#define CL_INVALID_COMMAND_QUEUE                    -36
#define CL_INVALID_HOST_PTR                         -37
#define CL_INVALID_MEM_OBJECT                       -38
#define CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          -39
#define CL_INVALID_IMAGE_SIZE                       -40
#define CL_INVALID_SAMPLER                          -41
#define CL_INVALID_BINARY                           -42
#define CL_INVALID_BUILD_OPTIONS                    -43
#define CL_INVALID_PROGRAM                          -44
#define CL_INVALID_PROGRAM_EXECUTABLE               -45
#define CL_INVALID_KERNEL_NAME                      -46
#define CL_INVALID_KERNEL_DEFINITION                -47
#define CL_INVALID_KERNEL                           -48
#define CL_INVALID_ARG_INDEX                        -49
#define CL_INVALID_ARG_VALUE                        -50
#define CL_INVALID_ARG_SIZE                         -51
#define CL_INVALID_KERNEL_ARGS                      -52
#define CL_INVALID_WORK_DIMENSION                   -53
#define CL_INVALID_WORK_GROUP_SIZE                  -54
#define CL_INVALID_WORK_ITEM_SIZE                   -55
#define CL_INVALID_GLOBAL_OFFSET                    -56
#define CL_INVALID_EVENT_WAIT_LIST                  -57
#define CL_INVALID_EVENT                            -58
#define CL_INVALID_OPERATION                        -59
#define CL_INVALID_GL_OBJECT                        -60
#define CL_INVALID_BUFFER_SIZE                      -61
#define CL_INVALID_MIP_LEVEL                        -62
#define CL_INVALID_GLOBAL_WORK_SIZE                 -63
#define CL_INVALID_PROPERTY                         -64

void printErrorByCode(int code) {
    switch(code) {
        case CL_SUCCESS:
            printf("CL_SUCCESS\n");
            break;
        case CL_DEVICE_NOT_FOUND:
            printf("CL_DEVICE_NOT_FOUND\n");
            break;
        case CL_DEVICE_NOT_AVAILABLE:
            printf("CL_DEVICE_NOT_AVAILABLE\n");
            break;
        case CL_COMPILER_NOT_AVAILABLE:
            printf("CL_COMPILER_NOT_AVAILABLE\n");
            break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            printf("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
            break;
        case CL_OUT_OF_RESOURCES:
            printf("CL_OUT_OF_RESOURCES\n");
            break;
        case CL_OUT_OF_HOST_MEMORY:
            printf("CL_OUT_OF_HOST_MEMORY\n");
            break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            printf("CL_PROFILING_INFO_NOT_AVAILABLE\n");
            break;
        case CL_MEM_COPY_OVERLAP:
            printf("CL_MEM_COPY_OVERLAP\n");
            break;
        case CL_IMAGE_FORMAT_MISMATCH:
            printf("CL_IMAGE_FORMAT_MISMATCH\n");
            break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            printf("CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
            break;
        case CL_BUILD_PROGRAM_FAILURE:
            printf("CL_BUILD_PROGRAM_FAILURE\n");
            break;
        case CL_MAP_FAILURE:
            printf("CL_MAP_FAILURE\n");
            break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            printf("CL_MISALIGNED_SUB_BUFFER_OFFSET\n");
            break;
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n");
            break;
        case CL_INVALID_VALUE:
            printf("CL_INVALID_VALUE\n");
            break;
        case CL_INVALID_DEVICE_TYPE:
            printf("CL_INVALID_DEVICE_TYPE\n");
            break;
        case CL_INVALID_PLATFORM:
            printf("CL_INVALID_PLATFORM\n");
            break;
        case CL_INVALID_DEVICE:
            printf("CL_INVALID_DEVICE\n");
            break;
        case CL_INVALID_CONTEXT:
            printf("CL_INVALID_CONTEXT\n");
            break;
        case CL_INVALID_QUEUE_PROPERTIES:
            printf("CL_INVALID_QUEUE_PROPERTIES\n");
            break;
        case CL_INVALID_COMMAND_QUEUE:
            printf("CL_INVALID_COMMAND_QUEUE\n");
            break;
        case CL_INVALID_HOST_PTR:
            printf("CL_INVALID_HOST_PTR\n");
            break;
        case CL_INVALID_MEM_OBJECT:
            printf("CL_INVALID_MEM_OBJECT\n");
            break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
            break;
        case CL_INVALID_IMAGE_SIZE:
            printf("CL_INVALID_IMAGE_SIZE\n");
            break;
        case CL_INVALID_SAMPLER:
            printf("CL_INVALID_SAMPLER\n");
            break;
        case CL_INVALID_BINARY:
            printf("CL_INVALID_BINARY\n");
            break;
        case CL_INVALID_BUILD_OPTIONS:
            printf("CL_INVALID_BUILD_OPTIONS\n");
            break;
        case CL_INVALID_PROGRAM:
            printf("CL_INVALID_PROGRAM\n");
            break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            printf("CL_INVALID_PROGRAM_EXECUTABLE\n");
            break;
        case CL_INVALID_KERNEL_NAME:
            printf("CL_INVALID_KERNEL_NAME\n");
            break;
        case CL_INVALID_KERNEL_DEFINITION:
            printf("CL_INVALID_KERNEL_DEFINITION\n");
            break;
        case CL_INVALID_KERNEL:
            printf("CL_INVALID_KERNEL\n");
            break;
        case CL_INVALID_ARG_INDEX:
            printf("CL_INVALID_ARG_INDEX\n");
            break;
        case CL_INVALID_ARG_VALUE:
            printf("CL_INVALID_ARG_VALUE\n");
            break;
        case CL_INVALID_ARG_SIZE:
            printf("CL_INVALID_ARG_SIZE\n");
            break;
        case CL_INVALID_KERNEL_ARGS:
            printf("CL_INVALID_KERNEL_ARGS\n");
            break;
        case CL_INVALID_WORK_DIMENSION:
            printf("CL_INVALID_WORK_DIMENSION\n");
            break;
        case CL_INVALID_WORK_GROUP_SIZE:
            printf("CL_INVALID_WORK_GROUP_SIZE\n");
            break;
        case CL_INVALID_WORK_ITEM_SIZE:
            printf("CL_INVALID_WORK_ITEM_SIZE\n");
            break;
        case CL_INVALID_GLOBAL_OFFSET:
            printf("CL_INVALID_GLOBAL_OFFSET\n");
            break;
        case CL_INVALID_EVENT_WAIT_LIST:
            printf("CL_INVALID_EVENT_WAIT_LIST\n");
            break;
        case CL_INVALID_EVENT:
            printf("CL_INVALID_EVENT\n");
            break;
        case CL_INVALID_OPERATION:
            printf("CL_INVALID_OPERATION\n");
            break;
        case CL_INVALID_GL_OBJECT:
            printf("CL_INVALID_GL_OBJECT\n");
            break;
        case CL_INVALID_BUFFER_SIZE:
            printf("CL_INVALID_BUFFER_SIZE\n");
            break;
        case CL_INVALID_MIP_LEVEL:
            printf("CL_INVALID_MIP_LEVEL\n");
            break;
        case CL_INVALID_GLOBAL_WORK_SIZE:
            printf("CL_INVALID_GLOBAL_WORK_SIZE\n");
            break;
        case CL_INVALID_PROPERTY:
            printf("CL_INVALID_PROPERTY\n");
            break;
        default:
            printf("Wrong code\n");
            break;
    }
}
