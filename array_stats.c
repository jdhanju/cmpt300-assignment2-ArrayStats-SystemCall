#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

struct array_stats_s {
	long min;
	long max;
	long sum;
};

SYSCALL_DEFINE3(array_stats, struct array_stats_s *, stats, long *, data, long,
		size)
{
	//variables
	int i = 0;
	int bytes_not_copied = 0;
	int firstIteration = 0;
	long userCheck = 0;
	//long arrayData = 0;

	//printk("over here!\n")

	//end the syscall if the size given is less than or equal to zero
	if (size <= 0) {
		printk("Size must me greater than zero and you gave: %ld\n",
		       size);
		return -EINVAL;
	}


	for (i = 0; i < size; i++) {
		long arrayData;
		bytes_not_copied =
			copy_from_user(&arrayData, &data[i], sizeof(long));
		//printk("Bytes not copied is: %d\n", bytes_not_copied);

		if (bytes_not_copied >
		    0) { //stops program if the given a wrong array pointer
			printk("Can't use data at index: %d\n", i);
			return -EFAULT;
		}

		//printk("Value of arrayData is: %ld", arrayData);
		//printk("Value of array is: %ld", data[i]);

		if (firstIteration == 0) {
			if (copy_to_user(&stats->min, &arrayData,
					 sizeof(long)) != 0) {
				return -EFAULT;
			}
			if (copy_to_user(&stats->max, &arrayData,
					 sizeof(long)) != 0) {
				return -EFAULT;
			}
			if (copy_to_user(&stats->sum, &arrayData,
					 sizeof(long)) != 0) {
				return -EFAULT;
			}

			firstIteration = 1;
		} else {
			if (copy_from_user(&userCheck, &stats->min,
					   sizeof(long)) != 0) {
				return -EFAULT;
			}
			if (userCheck > arrayData) {
				if (copy_to_user(&stats->min, &arrayData,
						 sizeof(long)) != 0) {
					return -EFAULT;
				}
			}

			if (copy_from_user(&userCheck, &stats->max,
					   sizeof(long)) != 0) {
				return -EFAULT;
			}
			if (userCheck < arrayData) {
				if (copy_to_user(&stats->max, &arrayData,
						 sizeof(long)) != 0) {
					return -EFAULT;
				}
			}

			if (copy_from_user(&userCheck, &stats->sum,
					   sizeof(long)) != 0) {
				return -EFAULT;
			}

            userCheck = userCheck + arrayData;

			if (copy_to_user(&stats->sum, &userCheck,
					 sizeof(long)) != 0) {
				return -EFAULT;
			}

			//stats->sum = stats->sum + arrayData;
		}
	}
	printk("Leaving array_stats syscall: Was successful!\n");
	return 0;
}