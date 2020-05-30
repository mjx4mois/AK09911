/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     File Name		: datatype_Layer.h
     Function		: porting DataType
     Create Date	: 2020/07/14
---------------------------------------------------------------------- */
#ifndef __DATATYPE_LAYER_HEADER___
#define __DATATYPE_LAYER_HEADER___

/*
|-------------------------------------------------------------------|
| Remame	|         Name         |    Size     	|            Range            			|
|-------------------------------------------------------------------|
|n/a		|     bit              	|    ? byte	|    0 or 1                   			|
|BOOL    		|     bool             	|    1 byte   	|    0 or 1                   			|
|CHAR8S  	|     char             	|    1 byte   	|    -128 ~ 127               		|
|CHAR8U  	|     unsigned char	|    1 byte   	|    0    ~ 255               		|
|n/a     		|     signed char	|    1 byte   	|    -128   ~ 127             		|
|INT16S  	|     int			|    2 byte   	|    -32768 ~ 32767           		|
|n/a     		|     short int		|    2 byte   	|    -32768 ~ 32767           		|
|INT16U  	|     unsigned int	|    2 byte   	|    0 ~ 65535                		|
|n/a     		|     signed int		|    2 byte   	|    -32768 ~ 32767           		|
|INT32S  	|     long int		|    4 byte   	|    -2147483648 ~ 2147483647 	|
|INT32U  	|     unsigned long	|    4 byte   	|    0 ~ 4294967295           		|
|n/a     		|     signed long	|    4 byte	|    -2147483648 ~ 2147483647 	|
|FLOAT   		|     float			|    4 byte	|    กำ1.175e-38 ~ กำ3.402e38   	|
|DOUBLE  	|     double		|    4 byte	|    กำ1.175e-38 ~ กำ3.402e38   	|
|--------------------------------------------------------------------
*/

#define  BOOL				bool
#define  CHAR8S 			char
#define  CHAR8U			unsigned char
#define  INT16S			int
#define  INT16U			unsigned int
#define  INT32S     		long int
#define  INT32U    			unsigned long int
#define  FLOAT     			float
#define  DOUBLE     		double

#endif		/*#ifndef __DATATYPE_LAYER_HEADER___ */