/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     File Name		: swi2c_Layer.c
     Function		: porting SWI2C API Function 
     Create Date	: 2020/07/14
---------------------------------------------------------------------- */

#include <mega32a.h>
#include <i2c.h>
#include <delay.h>
#include <stdio.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>



/*--------------------------------------------------------------------------------------------------*/
void i2c_stop_hang(void)
{    
	CHAR8U try_count;

		for(try_count=0;try_count<=3;try_count++)
		 {
			i2c_stop();
			delay_us(80);
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_write_1_byte_data(CHAR8U slave_address,CHAR8U register_address,CHAR8U data)
{
	CHAR8S ack=0,busy=0;
	
		i2c_stop();
		busy = i2c_start();
	
		if(busy)
		{
        		delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
		
			if(ack == 1)
			{
				ack=i2c_write(register_address);
				if(ack == 1)
				{

					ack=i2c_write(data);
					if(ack == 1)
                       		{
						i2c_stop();
						delay_us(10);
						return SWI2C_STATUS_OK;
                       		}
                       		else
                       		{
						printf("data error\r\n");
						goto EXIT;
					}
                   		}
                   		else
                   		{
                        		printf("register error\r\n");
                        		goto EXIT;
                   		}
            		}
            		else
            		{
                		printf("address error\r\n");
                		goto EXIT;
            		}
		}
     		else
     		{
    			EXIT:
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_FAIL;
     		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_write_n_byte_data(CHAR8U slave_address,CHAR8U start_reg_addr,CHAR8U n_byte,CHAR8U *w_data_stream)
{

	CHAR8S ack=0,busy=0,data_number,data_buffer=0;

		if(n_byte>255)
		{
			return SWI2C_STATUS_FAIL;/*over max set 255!!*/
		}
		else if(n_byte == 1)
		{
			return SWI2C_STATUS_FAIL;/* use i2c_write_1_byte_data() */
		}
	  
		i2c_stop();
		busy = i2c_start();
	
		if(busy)
       	{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
		
			if(ack == 1)
            		{
				ack=i2c_write(start_reg_addr);
				if(ack == 1)
                   		{

					for(data_number=0;data_number<n_byte;data_number++)
                    			{

	                            	data_buffer = w_data_stream[data_number];
	                            	ack = i2c_write(data_buffer);
	                            	delay_us(100); /*a little delay.*/

                                    	if(ack != 1)  /*if any byte write fail -> immediately return FAIL*/
                                    	{
							printf("write data sequenct fail error\r\n");		/*20200419 fix : use the same sequence -> goto */
							goto EXIT;
                                     	}

					}

					/*ALL write finish!!*/
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_OK;

				}
                   		else
				{
					printf("register error\r\n");
					goto EXIT;
                   		}
            		}
            		else
	            	{
	                	printf("address error\r\n");
	                	goto EXIT;
			}
		}
     		else
		{
    			EXIT:
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_FAIL;
     		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_read_1_byte_data(CHAR8U slave_address,CHAR8U register_address,CHAR8U *data)
{
      CHAR8S ack=0,busy=0;

		i2c_stop();
		busy = i2c_start();
	  
      		if(busy)
		{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
		
			if(ack == 1)
			{
				ack=i2c_write(register_address);
				if(ack == 1)
				{
					busy = i2c_start();
					if(busy)
                           		{
						ack = i2c_write(slave_address | READ_BIT);
						if(ack == 1)
                              		{
							*data=i2c_read(MASTER_NACK);
							i2c_stop();
							delay_us(10);

							return SWI2C_STATUS_OK;
						}
                           		}
                           		else
                           		{
						printf("register error\r\n");
						goto EXIT;
                            	}

				}
                   		else
                   		{
					printf("register error\r\n");
					goto EXIT;
                   		}
            		}
            		else
            		{
                		printf("address error\r\n");
                		goto EXIT;
            		}
     		}
     		else
     		{
			EXIT:
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_FAIL;
     		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_read_n_byte_data(CHAR8U slave_address,CHAR8U start_reg_addr,CHAR8U n_byte,CHAR8U *r_data_stream)
{

	CHAR8S ack=0,busy=0,data_number;


      		if(n_byte>255)
	  	{
			return SWI2C_STATUS_FAIL; /*over max set 255!!*/
              }
	  	else if(n_byte == 1)
		{
			return SWI2C_STATUS_FAIL;  /* use i2c_read_1_byte_data() */
	  	}
	  
      		i2c_stop();
      		busy = i2c_start();
	  
      		if(busy)
       	{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
		
			if(ack == 1)
			{
				ack=i2c_write(start_reg_addr);
				if(ack == 1)
				{
					busy = i2c_start();
					if(busy)
                           		{
						ack = i2c_write(slave_address | READ_BIT);
						if(ack == 1)
						{

							for(data_number=0;data_number<n_byte;data_number++)
							{

								if(data_number == (n_byte-1))	 /* 20200419 fix the (n_byte) to (n_byte-1) */ 
								{					
									r_data_stream[data_number] = i2c_read(MASTER_NACK);
									delay_us(3); /*a little delay.*/ 
									break;			
								}
								else
								{
							 		r_data_stream[data_number] = i2c_read(MASTER_ACK);
							 		delay_us(3); /*a little delay.*/ 
								}
					     		}
							
						      /*ALL read finish!!*/ 
			                           i2c_stop();
			                           delay_us(10);					
			                           return SWI2C_STATUS_OK;

                              		}
						else
                           			{
                             			printf("address error\r\n");
                             			goto EXIT;
                            		}
                           		}
                          		else
                           		{
						printf("register error\r\n");
						goto EXIT;
                            	}

                   		}
                   		else
                   		{
					printf("register error\r\n");
					goto EXIT;
                   		}
            		}
            		else
            		{
				printf("address error\r\n");
				goto EXIT;
            		}
     		}
     		else
     		{
			EXIT:
					i2c_stop();
					delay_us(10);
	  				return SWI2C_STATUS_FAIL; 	
     		}

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_write_1_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U data)
{
	CHAR8S ack=0,busy=0;

		i2c_stop();
		busy = i2c_start();
	
		if(busy)
		{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
			if(ack == 1)
			{
				ack=i2c_write(reg_high_addr);	
				if(ack == 1)
				{
					ack=i2c_write(reg_low_addr);
					if(ack == 1)
					{
						ack=i2c_write(data);
						if(ack == 1)
						{
							i2c_stop();
							delay_us(10);	
							return SWI2C_STATUS_OK;
						}
						else
						{
							printf("data error\r\n");
							goto EXIT;

						}
					}
					else
					{
						printf("register error\r\n");
						goto EXIT;
					}
				}
				else
				{
					printf("register error\r\n");
					goto EXIT;
				}

			}
			else
			{
				printf("address error\r\n");
				goto EXIT;
			}
	  	}
	  	else
	  	{
			EXIT:
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_FAIL;
	  	}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_write_n_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U n_byte,CHAR8U *w_data_stream)
{

	CHAR8S ack=0,busy=0,data_number,data_buffer=0;

		if(n_byte>255)
		{
			return SWI2C_STATUS_FAIL;  /*over max set 255!! */
		}
		else if(n_byte == 1)
		{
			return SWI2C_STATUS_FAIL;  /* use i2c_write_1_byte_data_two_reg_addr() */
		}
		  
		i2c_stop();
		busy = i2c_start();
		if(busy)
		{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
			if(ack == 1)
			{
				ack=i2c_write(reg_high_addr);
				if(ack == 1)
				{
					ack=i2c_write(reg_low_addr);
					if(ack == 1)
					{

						for(data_number=0;data_number<n_byte;data_number++)
						{

							data_buffer = w_data_stream[data_number];
		                            	ack = i2c_write(data_buffer);

							delay_us(100); /*a little delay. */

	                                   	if(ack != 1)  /*if any byte write fail -> immediately return FAIL */
	                                    	{
								printf("write data sequenct fail error\r\n");		/*20200419 fix : use the same sequence -> goto */
								goto EXIT;
	                                     	}

						}
						
						/* ALL write finish!! */
						i2c_stop();
						delay_us(10);
						return SWI2C_STATUS_OK;
						
	                   		}
	                   		else
	                   		{
						printf("register error\r\n");
						goto EXIT;
					}
				}
	              	else
	              	{
					printf("register error\r\n");
					goto EXIT;
	              	}
			}
			else
			{
				printf("address error\r\n");
				goto EXIT;
	
			}
		}
		else
		{
			EXIT:
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_FAIL;
		}

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_read_1_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U *data)
{

	CHAR8S ack=0,busy=0;

		i2c_stop();
		busy = i2c_start();
		if(busy)
		{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
			if(ack == 1)
			{
				ack=i2c_write(reg_high_addr);
				if(ack == 1)
				{
					ack=i2c_write(reg_low_addr);
					if(ack == 1)
	                      	{
						busy = i2c_start();
						if(busy)
	                           		{
							ack = i2c_write(slave_address | READ_BIT);
							if(ack == 1)
							{
								*data=i2c_read(MASTER_NACK);
								i2c_stop();
								delay_us(10);
								return SWI2C_STATUS_OK;
							}
	                           		}
	                          		else
	                           		{
							printf("register error\r\n");
							goto EXIT;
	                            	}
	                   		}
	                   		else
	                   		{
	                       		printf("register error\r\n");
	                        		goto EXIT;
	                   		}
                     	}
				else
	              	{
	                   		printf("register error\r\n");
	                   		goto EXIT;
	              	}	
			}
			else
			{
				printf("address error\r\n");
				goto EXIT;
			}
		}
		else
		{
			EXIT:
				i2c_stop();
				delay_us(10);
				return SWI2C_STATUS_FAIL;
	}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S i2c_read_n_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U n_byte,CHAR8U *r_data_stream)
{
	CHAR8S ack=0,busy=0,data_number;

		if(n_byte>255)
		{
			return SWI2C_STATUS_FAIL; /*over max set 255!!*/ 
		}
		else if(n_byte == 1)
		{
			return SWI2C_STATUS_FAIL;  /* use i2c_read_1_byte_data_two_reg_addr() */
		}
	
		i2c_stop();
		busy = i2c_start();
		if(busy)
		{
			delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
			if(ack == 1)
			{
				ack=i2c_write(reg_high_addr);
				if(ack == 1)
				{
					ack=i2c_write(reg_low_addr);
					if(ack == 1)
					{
						busy = i2c_start();
						if(busy)
						{
							ack = i2c_write(slave_address | READ_BIT);
							if(ack == 1)
							{

								for(data_number=0;data_number<n_byte;data_number++)
								{

									if(data_number == (n_byte-1))	 /* 20200419 fix the (n_byte) to (n_byte-1) */ 
									{
										r_data_stream[data_number] = i2c_read(MASTER_NACK);
										delay_us(3); /*a little delay.*/ 
										break;
									}
									else
									{
										r_data_stream[data_number] = i2c_read(MASTER_ACK);
										delay_us(3); /*a little delay.*/ 
									}
								}
								
							     /*ALL read finish!!*/ 
				                           i2c_stop();
				                           delay_us(10);
				                           return SWI2C_STATUS_OK;

	                              		}
							else
	                           			{
								printf("address error\r\n");
								goto EXIT;
	                            		}
	                           		}
	                           		else
	                           		{
							printf("register error\r\n");
							goto EXIT;
	                            	}
					 }
	                      	else
	                      	{
						printf("register error\r\n");
						goto EXIT;
	                      	}
	                   	}
	                  	 else
	                   	{
	                       	printf("register error\r\n");
	                        	goto EXIT;
	                   	}
	            	}
	            	else
	            	{
	                	printf("address error\r\n");
	                	goto EXIT;
	            	}
	     	}
	     	else
	     	{
			EXIT:
					i2c_stop();
					delay_us(10);
					return SWI2C_STATUS_FAIL;
		}
}
/*--------------------------------------------------------------------------------------------------*/