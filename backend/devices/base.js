'use strict'

class BaseDevice {
  constructor (deviceModel, deviceId, sendFn) {
    this._deviceModel = deviceModel
    this._deviceId = deviceId
    this._sendFn = sendFn
  }

  /**
   * Get device id
   */
  get deviceId () {
    return this._deviceId
  }

  /**
   * Get device model
   */
  get deviceModel () {
    return this._deviceModel
  }

  /**
   * Data received callback, you might rewrite this method
   *
   * @param {Buffer} data
   *
   * @return {(Buffer|undefined|null)} Send data to client automatically if return value is a Buffer
   */
  async received (data) {
  }

  /**
   * Send data to client
   *
   * @param {Buffer} data
   */
  async send (data) {
    await this._sendFn(data)
  }
}

module.exports = BaseDevice
