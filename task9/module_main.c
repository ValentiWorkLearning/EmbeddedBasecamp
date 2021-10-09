#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/list.h>
#include <linux/completion.h>
#include <linux/slab.h>

LIST_HEAD(task_list);
LIST_HEAD(completitions_list);
LIST_HEAD(threads_list);

typedef struct task_descriptor
{
   const char *message;
   int delay_message;
   int total_time;

   struct completion *p_completition;

   struct list_head list;
} task_descriptor_t;

typedef struct completition_data
{
   struct completion completition_handler;
   struct list_head list;
} completition_data_t;

typedef struct thread_descriptor
{
   struct task_struct *task_node;
   struct list_head list;
} thread_descriptor_t;

void add_task(const char *message, int delay_message, int total_time)
{
   task_descriptor_t *new_task = kmalloc(sizeof(task_descriptor_t), GFP_KERNEL);
   if (!new_task)
      return;

   const size_t required_memory_for_string = strlen(message) + 1;
   char *string_buffer = kmalloc(required_memory_for_string, GFP_KERNEL);
   if (!string_buffer)
   {
      kfree(new_task);
      return;
   }

   strcpy(string_buffer, message);
   new_task->message = string_buffer;
   new_task->delay_message = delay_message;
   new_task->total_time = total_time;

   completition_data_t *p_complete_handler = kmalloc(sizeof(completition_data_t), GFP_KERNEL);
   init_completion(&p_complete_handler->completition_handler);

   new_task->p_completition = &p_complete_handler->completition_handler;
   list_add_tail(&new_task->list, &task_list);
   list_add_tail(&p_complete_handler->list, &completitions_list);
}

static int thread_func(void *data)
{
   task_descriptor_t *thread_data = (task_descriptor_t *)data;
   int remaining_process = thread_data->total_time;
   while (remaining_process > 0)
   {
      msleep(thread_data->delay_message);
      printk("Task delay_time :%d, message: %s\n", thread_data->delay_message, thread_data->message);
      remaining_process -= thread_data->delay_message;
   }
   complete(thread_data->p_completition);
   return 0;
}

void task_dtor(task_descriptor_t *task)
{
   if (task)
   {
      kfree(task->message);
      kfree(task);
   }
}

void thread_dtor(thread_descriptor_t *thread)
{
   if (thread)
      kfree(thread);
}

void completition_dtor(completition_data_t *completition)
{
   if (completition)
      kfree(completition);
}

void cleanup_tasks(void)
{
   task_descriptor_t *task_descriptor = NULL;
   struct list_head *pos, *q;
   list_for_each_safe(pos, q, &task_list)
   {
      task_descriptor = list_entry(pos, task_descriptor_t, list);
      list_del(pos);
      task_dtor(task_descriptor);
   }
}

void cleanup_completitions(void)
{
   completition_data_t *completition_it = NULL;
   struct list_head *pos, *q;
   list_for_each_safe(pos, q, &completitions_list)
   {
      completition_it = list_entry(pos, completition_data_t, list);
      list_del(pos);
      completition_dtor(completition_it);
   }
}

void cleanup_threads(void)
{
   thread_descriptor_t *thread_it = NULL;
   struct list_head *pos, *q;
   list_for_each_safe(pos, q, &threads_list)
   {
      thread_it = list_entry(pos, thread_descriptor_t, list);
      list_del(pos);
      thread_dtor(thread_it);
   }
}
void cleanup_items(void)
{
   cleanup_tasks();
   cleanup_completitions();
   cleanup_threads();
}
static int test_module(void)
{
   printk("ADDING TASKS");
   add_task("test task1", 70, 100);
   add_task("test task2", 40, 100);
   add_task("test task5", 10, 100);

   printk("ADDING THREADS BEGIN");
   task_descriptor_t *task_it = NULL;
   list_for_each_entry(task_it, &task_list, list)
   {

      thread_descriptor_t *p_thread = kmalloc(sizeof(thread_descriptor_t), GFP_KERNEL);
      p_thread->task_node = kthread_run(thread_func, (void *)task_it, "WORKER");
      list_add_tail(&p_thread->list, &threads_list);
   }
   printk("ADDING THREADS END");

   printk("WAITING FOR COMPLETE BEGIN");
   completition_data_t *completition_it = NULL;
   list_for_each_entry(completition_it, &completitions_list, list)
   {
      wait_for_completion(&completition_it->completition_handler);
   }
   printk("WAITING FOR COMPLETE END");
   cleanup_items();
   printk("!Module finished\n");
   return -1;
}

module_init(test_module);
MODULE_LICENSE("GPL");
