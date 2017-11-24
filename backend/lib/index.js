'use strict'

const fs = require('fs')
const path = require('path')
const net = require('net')
const { Transport } = require('./transport')
const devicesConfig = require('../config/devices.json')

class Client {
  constructor (client) {
    this._trans = new Transport(client)
    this._deviceCache = {}
  }

  async run () {
    while (true) {
      const frame = await this._read()
      const instance = this._requireInstance(frame)

      this._callReceived(frame, instance)
    }
  }

  _callReceived (frame, instance) {
    instance.received(frame.data)
      .then(response => {
        if (response) {
          instance.send(response)
        }
      })
      .catch(err => {
        console.log(err)
      })
  }

  _requireInstance (frame) {
    let cachedInstance = this._deviceCache[this._genCacheKey(frame)]

    /* check if cache exists */
    if (!cachedInstance) {
      cachedInstance = this._createDeviceInstance(frame)
      this._deviceCache[this._genCacheKey(frame)] = cachedInstance
    }

    return cachedInstance
  }

  _createDeviceInstance (frame) {
    /* get device module name */
    let moduleName = Object.keys(devicesConfig).find(key => frame.model === devicesConfig[key])

    /* get class */
    const Cls = require(path.resolve('devices', moduleName))

    return new Cls(frame.model, frame.id, data => {
      const writeFrame = Object.assign(frame, {
        data,
        length: data.length
      })

      this._trans.write(writeFrame)
    })
  }

  _genCacheKey (frame) {
    return `${frame.model}-${frame.id}`
  }

  async _read () {
    return this._trans.read()
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
