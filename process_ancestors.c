#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

#define ANCESTOR_NAME_LEN 16
struct process_info {
	long pid; /* Process ID */
	char name[ANCESTOR_NAME_LEN]; /* Program name of process */
	long state; /* Current process state */
	long uid; /* User ID of process owner */
	long nvcsw; /* # voluntary context switches */
	long nivcsw; /* # involuntary context switches */
	long num_children; /* # children process has */
	long num_siblings; /* # sibling process has */
};
SYSCALL_DEFINE3(process_ancestors, /* syscall name for macro */
		struct process_info *,
		info_array, /* array of process info strct */
		long, size, /* size of the array */
		long *, num_filled) /* # elements written to array */
{
	// your code here…
	long pid; /* Process ID */
	char name[ANCESTOR_NAME_LEN]; /* Program name of process */
	long state; /* Current process state */
	long uid; /* User ID of process owner */
	long nvcsw; /* # voluntary context switches */
	long nivcsw; /* # involuntary context switches */
	long num_children; /* # children process has */
	long num_siblings; /* # sibling process has */
	int i = 0;

	struct task_struct *curr_task = current;
	struct task_struct *startingPoint = current;
	pid = curr_task->pid;
	state = curr_task->state;
	nvcsw = curr_task->nvcsw;
    nivcsw = curr_task->nivcsw;

	for (i = 0; i < size; i++) {
		//copy pid
		if (copy_to_user(&info_array[i].pid, &pid, sizeof(long)) != 0) {
			return -EFAULT;
		}

		//copy the process name
		if (copy_to_user(&info_array[i].name, &curr_task->comm,
				 sizeof(char) * 16) != 0) {
			return -EFAULT;
		}

		//copy the state of the process
		if (copy_to_user(&info_array[i].state, &state, sizeof(long)) !=
		    0) {
			return -EFAULT;
		}

		//copy the nvcsw of the process
		if (copy_to_user(&info_array[i].nvcsw, &nvcsw, sizeof(long)) !=
		    0) {
			return -EFAULT;
		}

		//copy the nivcsw of the process
		if (copy_to_user(&info_array[i].nivcsw, &nivcsw, sizeof(long)) !=
		    0) {
			return -EFAULT;
		}

		if (pid == 0) {
			i++;
			if (copy_to_user(num_filled, &i, sizeof(int)) != 0) {
				return -EFAULT;
			}
			printk("Number of cycles: %d\n", i);
			return 0;
		}

		curr_task = curr_task->parent;
		pid = curr_task->pid;
		state = curr_task->state;
		nvcsw = curr_task->nvcsw;
        nivcsw = curr_task->nivcsw;
	}

	// printk("pid is: %d\n", current->pid);
	// pid = current->pid;
	// if (copy_to_user(&info_array[0].pid, &pid, sizeof(long)) !=
	//     0) {
	// 	return -EFAULT;
	// }

	// if (copy_to_user(&info_array[0].name, &current->comm, sizeof(char)*16) !=
	//     0) {
	// 	return -EFAULT;
	// }
	//     struct task_struct *curr_task = current->parent;
	//     pid = curr_task->pid;

	// 	if (copy_to_user(&info_array[1].pid, &pid, sizeof(long)) !=
	//     0) {
	// 	return -EFAULT;
	// }

	// if (copy_to_user(&info_array[1].name, &curr_task->comm, sizeof(char)*16) !=
	//     0) {
	// 	return -EFAULT;
	// }

	return 0;
}