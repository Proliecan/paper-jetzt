# paper-jetzt

A server for a mini game challenge.
Players connect via TCP and get a hint to go from there.

## Table of Contents

- [paper-jetzt](#paper-jetzt)
  - [Table of Contents](#table-of-contents)
  - [Deployment](#deployment)
  - [Development](#development)
  - [Dependencies](#dependencies)
    - [Building](#building)
    - [Running](#running)
      - [Server:](#server)
      - [Built-in bot:](#built-in-bot)
    - [Testing](#testing)
  - [Contributing](#contributing)
  - [License](#license)

## Deployment
Not ready for production yet.

## Development
## Dependencies
- [gcc](https://gcc.gnu.org/)
- [make](https://www.gnu.org/software/make/) are required to build the project.
- [boost](https://www.boost.org/)

### Building
```bash
make
```

### Running
#### Server:

```bash
out/bin/server
```

#### Built-in bot:

```bash
out/bin/bot
```

### Testing
Use (netcat)[https://en.wikipedia.org/wiki/Netcat] to send test packets to running test server.

## Contributing

Fork and merge feature branches into develop. Merge develop into main for releases.

## License

This project is licensed under the MIT License - see the LICENSE file for details