package main

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"time"
)

// This file is used as a server to tell the Apex board to start recording.
// It works on a pull based model i.e. the board connects to us, and repeatedly asks us if it can start, rather than us telling it to start.
// The reason for this is reliability, since if the board is broken or can't connect, we find out much sooner and can do something about it.

// General process
// 1. We start listening for tcp packets on a port (8080), which the board eventually connects to.
// 2. We also start listening for http packets on port 8081, which will be used by a mobile client to tell us to start.
// 3. When we get a start message, we'll tell the board to start next time it asks, it will then reply with an ACK message.
// 4. We'll relay that ACK to the mobile client, when it asks for the board status.
// 5. If anything goes wrong; the board doesn't ask us for 5 seconds after the initial connection, or fails to ACK the start message, we'll set the board status to ERROR

type boardStatus int

const (
	STATUS_WAITING = iota + 1
	STATUS_STARTED
	STATUS_BROKEN
)

var statuses = []string{
	"STATUS_WAITING",
	"STATUS_STARTED",
	"STATUS_BROKEN",
}

const clientTimeout = time.Second * 10

var BoardStatus boardStatus = STATUS_WAITING

var shouldStart = false

func handleStatus(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, statuses[BoardStatus-1])
}

func handleStart(w http.ResponseWriter, r *http.Request) {
	shouldStart = true
	log.Println("Started")
}

func httpServer() {
	http.HandleFunc("/status", handleStatus)
	http.HandleFunc("/start", handleStart)
	http.ListenAndServe(":8081", nil)
}

func tcpServer() {
	listen, err := net.Listen("tcp", ":8080")
	if err != nil {
		log.Panic(err)
	}
	// close listener
	defer listen.Close()
	for {
		conn, err := listen.Accept()
		if err != nil {
			log.Panic(err)
		}
		fmt.Println("Client Connected")
		go handleIncomingRequest(conn)
	}
}

func handleIncomingRequest(conn net.Conn) {
	// close conn
	defer conn.Close()

	// store incoming data
	buffer := make([]byte, 1024)
	sentStartMessage := false
	for {
		conn.SetReadDeadline(time.Now().Add(clientTimeout))
		n, err := conn.Read(buffer)
		if err != nil {
			BoardStatus = STATUS_BROKEN
			log.Println(err)
			return
		}
		if n != 3 { //Send any 3 bytes just to see if everything works - probably don't need this check
			BoardStatus = STATUS_BROKEN
			log.Println("WARNING TCP packet didn't have 3 bytes, had", n)
			return
		}

		log.Println("HEALTHY")
		if sentStartMessage {
			BoardStatus = STATUS_STARTED
			return
		}
		if shouldStart {
			conn.Write([]byte("START\n"))
			sentStartMessage = true
		} else {
			conn.Write([]byte("WAIT\n"))
		}
	}
}

func main() {
	go httpServer()
	go tcpServer()

	for {
	}
}
