#include "timer.h"
#include "Network.h"
#include "Order_handler.h"

int main()
{
	order_data_t order_queue[100];


	/*init section*/
	network_init();
	//......

	/*void loop*/
	while (1)
	{
		if (timer_check_timeout)
		{
			order_update_queue();
		}
		if (network_new_message)
		{
			order_update_queue(network_get_new message());
		}

	}
}