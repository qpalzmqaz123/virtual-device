'use strict'

const fs = require('fs')
const net = require('net')
const { Transport } = require('./transport')

class Client {
  constructor (client) {
    this._trans = new Transport(client)
  }

  async _read () {
    return this._trans.read()
  }

  async run () {
    while (true) {
      let data = await this._read()
      console.log('ffff', data)
    }
  }
}

function _createServer (options, callback) {
  if (fs.existsSync(options.socketPath)) {
    fs.unlinkSync(options.socketPath)
  }

  const server = net.createServer(callback)

  server.on('error', err => {
    throw err
  })

  server.listen(options.socketPath, () => console.log('server bound'))
}

/**
 * Run server
 *
 * @param {Object} options
 * @param {string} options.socketPath - unix socket path
 */
exports.run = function (options) {
  _createServer(options, client => {
    console.log('Connection established')
    const ins = new Client(client)
    ins
      .run()
      .catch(err => {
        console.error(err)
        process.exit(1)
      })
  })
}
