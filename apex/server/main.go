package main

import (
	"fmt"
	"net/http"
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

var BoardStatus boardStatus = STATUS_WAITING

var shouldStart = false

func handleStatus(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, statuses[BoardStatus-1])
}

func handleStart(w http.ResponseWriter, r *http.Request) {
	shouldStart = true
	fmt.Println("Started")
}

func httpServer() {
	http.HandleFunc("/status", handleStatus)
	http.HandleFunc("/start", handleStart)
	http.ListenAndServe(":8081", nil)
}

func main() {
	go httpServer()

	for {
	}
}
