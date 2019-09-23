BINARY := conways-game

build:
	mkdir -p release
	GOOS=linux GOARCH=amd64 GOPATH=${CURDIR} go build -o release/$(BINARY).go src/main.go

clean:
	rm -rf release

fmt:
	gofmt -w src/**/*.go
