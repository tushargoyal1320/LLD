package main

import (
	"context"
	"errors"
	"fmt"
	"sync"
	"time"
)

/*
Use a worker pool when:

✔ You have millions of tasks

You cannot create millions of goroutines.

✔ Tasks are CPU-bound or IO-bound

Limit concurrency = better performance.

A Worker Pool = Limited workers + unlimited jobs → workers keep taking jobs until the queue is empty / stopped.
*/
type Job struct {
	id int
}

// we could define job as => type Job func()-> this allows us to pass any function to be submitted to the worker pool and it'll execute
// it with given constraints.

type WorkerPool struct {
	jobs   chan Job
	ctx    context.Context
	cancel context.CancelFunc
	wg     sync.WaitGroup
}

func NewWorkerPool(queueSize int, workers int) *WorkerPool {
	ctx, cancel := context.WithCancel(context.Background())
	workerPool := &WorkerPool{
		jobs:   make(chan Job, queueSize),
		ctx:    ctx,
		cancel: cancel,
	}

	for i := 0; i < workers; i++ {
		workerPool.wg.Add(1)
		go worker(workerPool.ctx, i, workerPool.jobs, &workerPool.wg)
	}
	return workerPool
}

//Simple to understand
// func worker(id int, jobs <-chan Job, wg *sync.WaitGroup) {
// 	defer wg.Done()

// 	for v := range jobs {
// 		fmt.Printf("Worker %d working on job %d\n", id, v)
// 		time.Sleep(50 * time.Millisecond)
// 	}
// }

func worker(ctx context.Context, id int, jobs <-chan Job, wg *sync.WaitGroup) {
	defer wg.Done()
	defer fmt.Printf("Worker %d stopped\n", id)
	/*
		This means workers keep looping and processing jobs until:

		shutdown requested OR

		channel closed and drained.
	*/
	for {
		select {
		case <-ctx.Done(): // blocks forever until cancel is called
			fmt.Printf("Worker %d stopping...\n", id)
			return
		case j, ok := <-jobs:
			if !ok {
				return
			}
			fmt.Printf("Worker %d working on job %d\n", id, j)
		}
	}
}

func Submit(workerPool *WorkerPool, job Job) error {
	select {
	case workerPool.jobs <- job:
		return nil
	default:
		return errors.New("queue is full")
	}
}

func Shutdown(wp *WorkerPool) {
	wp.cancel()    // this simply mean shutdown is requested
	close(wp.jobs) // this signal no more job to accept but will complete the available jobs in the channel
	wp.wg.Wait()   // simple making main func. not to exit before all go routines are executed
}

func main() {

	workerPool := NewWorkerPool(100, 5)

	for i := 0; i < 20; i++ {
		Submit(workerPool, Job{id: i})
	}

	time.Sleep(5 * time.Second)
	Shutdown(workerPool)
	fmt.Printf("All jobs executed")
}
