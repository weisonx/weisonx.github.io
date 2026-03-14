资料链接： https://www.cnblogs.com/fnlingnzb-learner/p/5835573.html

内核实现 https://www.baidu.com/link?url=6D9WG0-ivTqu97nwtH0SapcXI8Arznwlj_kr-JaaK4AoQfK3qKbo_J7L7oXybvj7TZ2u-bOHzl2ZSUT1vDEmEa&wd=&eqid=bf6a9d07002dbe6d00000006604e664a

注	fd：文件描述符号，类似文件句柄，file descriptor

epoll和select的区别	epoll和select都是用于多路复用的一些系统调用，可以监听多个文件描述符上的事件并进行处理。它们的主要区别在于：

    1. 所处理的文件描述符数量不同：select使用数组来存储文件描述符，所以其能够处理的文件描述符的数量受到限制；而epoll使用红黑树来存储文件描述符，所以其能够处理的文件描述符数量不受限制。

    2. 对于大量文件描述符的IO事件的处理效率不同：当需要同时处理大量的文件描述符时，epoll比select有更好的性能表现。这是因为在处理大量文件描述符时，epoll只需要遍历那些发生了事件的文件描述符，而select需要遍历所有的文件描述符。

    3. select有一些平台特定的限制：在某些平台上，select可能不支持某些类型的文件描述符，而epoll没有这样的限制。

    总之，尽管两者都可以实现多路复用，但在处理大量文件描述符时，epoll比select具有更好的性能和可扩展性，所以epoll更加常用
    
    来自 <https://chat.theb.ai/#/chat/1002> 
    
epoll和poll的区别	epoll是Linux内核提供的一种事件通知机制，可以用来实现I/O多路复用。

相比于poll，epoll具有更高的性能和可伸缩性。在处理大量连接的情况下，epoll可以极大地减少系统的开销。

    epoll的主要优点是：

    1. 没有最大文件描述符数量的限制，可以支持上万个连接；

    2. 支持水平触发和边缘触发两种工作模式；

    3. 将事件存储在内核空间中，避免了每次调用都需要将事件从用户空间拷贝到内核空间，提高了I/O效率；

    4. 可以使用mmap加速大规模数据传输。

    而poll则没有这些优势，它的最大限制是1024个文件描述符，同时poll每次调用都需要将事件从用户空间拷贝到内核空间，所以效率上会差一些。

    总的来说，如果你需要处理大量连接，那么使用epoll会更好一些。但如果你的连接数不是很多，那么使用poll也是可以的。
    
    来自 <https://chat.theb.ai/#/chat/1002> 
    
epoll为什么可以只遍历发生了事件的文件描述符（select和poll都只能轮询所有的）	epoll通过使用内核支持的事件机制，将所有需要监控的文件描述符存储在一个红黑树中，并使用一个链表来保存发生事件的文件描述符。这样，在检查是否有事件发生时，只需要遍历链表即可，而不用遍历整个红黑树。
    
    当一个文件描述符上发生了事件时，内核会把这个文件描述符加入到一个链表中，然后唤醒等待该事件的进程。进程得到通知后，可以直接从链表中取出发生事件的文件描述符。
    
    这种机制使得epoll在处理大量文件描述符时，性能更高。因为它不需要像select、poll那样每次都需要遍历整个文件描述符集，只需要遍历发生事件的文件描述符集合，就能够快速地找到并处理发生的事件。
    
    另外，epoll还支持水平触发和边缘触发两种工作模式。在水平触发模式下，只要文件描述符上有数据可读或可写，就会返回事件通知；而在边缘触发模式下，只有在文件描述符状态发生变化时才返回事件通知。这种机制也能够提高epoll的性能和效率。

在Linux的网络编程中，很长的时间都在使用select做事件触发。在新的Linux内核中，有新的机制--epoll
一、select

    1. 采用轮询处理fd，当fd数目多时，耗时越多
    2. select同时监听的数据默认被宏定义为1024.虽然可以重新编译内核以扩大这个数，但治标不治本

二、epoll
epoll的接口非常简单，一共就三个函数：

1. int epoll_create(int size); //参数size为int意味着其支持最大的fd数目为2^31-1
创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。

2. int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。第一个参数是epoll_create()的返回值，第二个参数表示动作，用三个宏来表示：
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
第三个参数是需要监听的fd，第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：

events可以是以下几个宏的集合：

EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；

EPOLLOUT：表示对应的文件描述符可以写；

EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；

EPOLLERR：表示对应的文件描述符发生错误；

EPOLLHUP：表示对应的文件描述符被挂断；

EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。

EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里

3. int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个 maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示已超时。

4.关于ET、LT两种工作模式：

可以得出这样的结论:

ET模式仅当状态发生变化的时候才获得通知,这里所谓的状态的变化并不包括缓冲区中还有未处理的数据,也就是说,如果要采用ET模式,需要一直read/write直到出错为止,很多人反映为什么采用ET模式只接收了一部分数据就再也得不到通知了,大多因为这样;而LT模式是只要有数据没有处理就会一直通知下去的.


那么究竟如何来使用epoll呢？其实非常简单。
通过在包含一个头文件#include <sys/epoll.h> 以及几个简单的API将可以大大的提高你的网络服务器的支持人数。

首先通过create_epoll(int maxfds)来创建一个epoll的句柄，其中maxfds为你epoll所支持的最大句柄数。这个函数会返回一个新的epoll句柄，之后的所有操作将通过这个句柄来进行操作。在用完之后，记得用close()来关闭这个创建出来的epoll句柄。

之后在你的网络主循环里面，每一帧的调用epoll_wait(int epfd, epoll_event events, int max events, int timeout)来查询所有的网络接口，看哪一个可以读，哪一个可以写了。基本的语法为：
nfds = epoll_wait(kdpfd, events, maxevents, -1);

其中kdpfd为用epoll_create创建之后的句柄，events是一个epoll_event*的指针，当epoll_wait这个函数操作成功之后，epoll_events里面将储存所有的读写事件。max_events是当前需要监听的所有socket句柄数。最后一个timeout是 epoll_wait的超时，为0的时候表示马上返回，为-1的时候表示一直等下去，直到有事件范围，为任意正整数的时候表示等这么长的时间，如果一直没有事件，则范围。一般如果网络主循环是单独的线程的话，可以用-1来等，这样可以保证一些效率，如果是和主逻辑在同一个线程的话，则可以用0来保证主循环的效率。

epoll_wait范围之后应该是一个循环，遍利所有的事件。
