'use strict';


const API_URL = `${location.origin}/api`


const ui = new UI()


const httpService = new HttpService({
    apiUrl: API_URL,
    timeout: 2000
})


const appModule = new AppModule(ui, httpService, { ...appModuleConfig })
