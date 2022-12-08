if(count <= 4)
{
	if (count < 4 && inputPin == 1)
	{
		startRecieved |= (inputPin << (4 - count));
		PORTB = startRecieved;
	}
	else if(count == 4 && inputPin == 0)
	{
		startRecieved |= inputPin;
		
	}
	else
	{
		startRecieved = 0;
		count = 0;
	}
}
else(count)




if ((count > 4) && (startRecieved == 0b00001110) && (count <= dataSIZE + 4))
{
	
	//PORTB = count;
	longPortA |= inputPin;
	if (count <= (dataSIZE + 4))
	{
		datapakkeRecieved = (datapakkeRecieved | (longPortA << (31 -(count -5))));
		PORTB = (datapakkeRecieved >> 24);
	}
}

else if ((count > 4) && (startRecieved != 0b00001110))
{
	startRecieved = 0;
	datapakkeRecieved = 0;
	count = 0;

}
//PORTB = startRecieved;
count++;