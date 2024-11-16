'use strict'


const API_URL = `${location.origin}/api`


const ui = new UI()


const httpService = new HttpService({
    apiUrl: API_URL,
    timeout: 1000
})


const appModule = new AppModule(ui, httpService, {
    offDelayUseMinutes: false,  // true минуты; false - секунды
    updateLightStatusInterval: 2000    
})
