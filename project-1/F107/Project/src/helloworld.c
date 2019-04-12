#include "helloworld.h"
#include "lwip/tcp.h"
#include <string.h>
#include <delay.h>

#define MAX_NAME_SIZE 32

struct name 
{
  int length;
  char bytes[MAX_NAME_SIZE];
};
static err_t HelloWorld_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static err_t HelloWorld_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static void HelloWorld_conn_err(void *arg, err_t err);

static err_t HelloWorld_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  struct pbuf *q;
  struct name *name = (struct name *)arg;
  int done;
  char *c;
  int i;
  if (p != NULL) 
  {        
	tcp_recved(pcb, p->tot_len);
	if(!name){
      pbuf_free(p);
      return ERR_ARG;
    }
    done = 0;
    for(q=p; q != NULL; q = q->next) 
    {
      c = q->payload;
      for(i=0; i<q->len && !done; i++) 
	  {
        done = ((c[i] == 'A'));
        if(name->length < MAX_NAME_SIZE) 
	    {
          name->bytes[name->length++] = c[i];
        }
      }
    }
    if(done) 
    {
      tcp_write(pcb, name->bytes, name->length, TCP_WRITE_FLAG_COPY);
      name->length = 0;
    }
    pbuf_free(p);
  }  
  else if (err == ERR_OK) 
  {
    mem_free(name);
    return tcp_close(pcb);
  }
  return ERR_OK;
}

static err_t HelloWorld_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{     
  tcp_arg(pcb, mem_calloc(sizeof(struct name), 1));	
  tcp_err(pcb, HelloWorld_conn_err);
  tcp_recv(pcb, HelloWorld_recv);
  return ERR_OK;
}

void HelloWorld_init(void)
{
  struct tcp_pcb *pcb;	            		
  pcb = tcp_new();	                		 	
  tcp_bind(pcb, IP_ADDR_ANY, 1031);       
  pcb = tcp_listen(pcb);					
  tcp_accept(pcb, HelloWorld_accept);   							
}

static void HelloWorld_conn_err(void *arg, err_t err)
{
  struct name *name;
  name = (struct name *)arg;

  mem_free(name);
}
