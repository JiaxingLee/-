var devicesId               //先定义我所需要的变量
var api_key
const app = getApp()         //接入aap.js里面的全局变量
//var datastreamId ="sunshinedoing"
var fancommand="0"
var watercommand="0"
var sunshinecommand="0"
Page({ 
  data:{                //我所要从onenet实时获取的data放在此处，下面用this.setData实时更改即可
    tempreture:"",
    airHumidity:"",
    soilHumidity:"",
    illumination:""
  },
  onLoad:function(options){       //所设置的全局变量在此处赋值给下面发送请求需要用到的设备id和apikey
    devicesId = app.globalData.devicesId
    api_key = app.globalData.api_key
    console.log(devicesId,api_key)
    var _this=this
    var interval = setInterval(function () {
      wx.request({
        url: `https://api.heclouds.com/devices/${devicesId}/datapoints`,
        data: {
           x: '',
          y: ''
        },
        header: {
          'content-type': 'application/json', // 默认值
          'api-key': api_key
        },
        success(res) {                                     //每秒刷新一次数据
          console.log(res.data)
          var result = res.data.data.datastreams[2].datapoints[0].value
          //console.log(result)
          _this.setData({
            tempreture: res.data.data.datastreams[0].datapoints[0].value,
            airHumidity:res.data.data.datastreams[2].datapoints[0].value,
            soilHumidity:res.data.data.datastreams[4].datapoints[0].value,
            illumination:res.data.data.datastreams[1].datapoints[0].value,
          })
        //  clearInterval(interval)           //调试阶段不用频繁地在控制台输出，调试完可注释掉
        }

      })

    }, 1000) //循环间隔 单位ms
  },

//点击启动排气扇
fan:function(){
  if (fancommand==0){          //点击即切换状态
    fancommand = 1;
  }
  else{
    fancommand=0;
  }
  wx.request({
    url: `https://api.heclouds.com/devices/${devicesId}/datapoints`,
    method: 'POST',
    header: {
      'content-type': 'application/json',
      'api-key': api_key,
    },
    data: {
      "datastreams": [{
        "id": "fandoing",
        "datapoints": [{
          "value": fancommand
        }]
      }]
    },
    success(res) {
      console.log("ok")
    }
  })
},
//点击打开水泵浇水灌溉
water:function(){
  if (watercommand == 0) {
    watercommand = 1;
  }
  else {
    watercommand = 0;
  }
  wx.request({
    url: `https://api.heclouds.com/devices/${devicesId}/datapoints`,
    method: 'POST',
    header: {
      'content-type': 'application/json',
      'api-key': api_key,
    },
    data: {
      "datastreams": [{
        "id": "waterdoing",
        "datapoints": [{
          "value": watercommand
        }]
      }]
    },
    success(res) {
      console.log("ok")
    }
  })
},
//点击调控遮阳或者拉开遮阳布
sunshine:function(){
  if (sunshinecommand == 0) {
    sunshinecommand = 1;
  }
  else {
    sunshinecommand = 0;
  }
  wx.request({
    url: `https://api.heclouds.com/devices/${devicesId}/datapoints`,
    method: 'POST',
    header: {
      'content-type': 'application/json',
      'api-key': api_key,
    },
    data: {
      "datastreams": [{
        "id": "sunshinedoing",
        "datapoints": [{
          "value": sunshinecommand
        }]
      }]
    },
    success(res) {
      console.log("ok")
    }
  })
}

})