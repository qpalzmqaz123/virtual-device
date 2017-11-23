'use strict'

class Transport {
  constructor (sock) {
    this._sock = sock
    this._buffer = Buffer.alloc(0)
  }

  _read () {
    return new Promise((resolve, reject) => {
      this._sock.once('data', chunk => resolve(chunk))
    })
  }

  _analyzeHeader () {
    if (this._buffer.length < 12) {
      return null
    }

    return {
      model: this._buffer.readUInt32LE(0),
      id: this._buffer.readUInt32LE(4),
      length: this._buffer.readUInt32LE(8)
    }
  }

  _getFrame () {
    const header = this._analyzeHeader()
    if (!header || this._buffer.length < 12 + header.length) {
      return null
    }

    const data = this._buffer.slice(12, 12 + header.length)
    this._buffer = this._buffer.slice(12 + header.length)

    return Object.assign(header, { data })
  }

  async read () {
    while (true) {
      let data = await this._read()
      this._buffer = Buffer.concat([ this._buffer, data ])

      const frame = this._getFrame()
      if (frame) {
        return frame
      }
    }
  }
}

exports.Transport = Transport
