const UART_SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";
const TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";
var UARTService = null;
var BLEDevice = null;

async function onConnBtnClick() {
    let opt = {
        filters: [{
            namePrefix: 'ESP32'
        },
        { services: ['battery_service', UART_SERVICE_UUID] }]
    };

    try {
        console.log('請求BLE裝置連線…');
        BLEDevice = await navigator.bluetooth.requestDevice(opt);

        $('#connBtn').attr('disabled', 'disabled');

        console.log('連接GATT伺服器…');
        const server = await BLEDevice.gatt.connect();
        BLEDevice.addEventListener('gattserverdisconnected', onDisconnected);

        console.log('取得電池服務…');
        const battService = await server.getPrimaryService('battery_service');

        console.log('取得電量特徵…');
        const battChar = await battService.getCharacteristic('battery_level');
        $('#controller').show(500);//or $(this).hide(1000);

        await battChar.startNotifications();

        battChar.addEventListener('characteristicvaluechanged',
            e => {
                let val = e.target.value.getUint8(0);
                $('#battLevel').text(val);

                if (val > 90) {
                    $('#battery > i').attr('class', 'las la-battery-full');
                } else if (val > 70) {
                    $('#battery > i').attr('class', 'las la-battery-three-quarters');
                } else if (val > 40) {
                    $('#battery > i').attr('class', 'las la-battery-half');
                } else if (val > 10) {
                    $('#battery > i').attr('class', 'las la-battery-quarter');
                } else {
                    $('#battery > i').attr('class', 'las la-battery-empty');
                }

            });

        console.log('取得UART服務…');
        UARTService = await server.getPrimaryService(UART_SERVICE_UUID);

    } catch (error) {
        $('#connBtn').removeAttr('disabled');
        if (BLEDevice != null) {
            if (BLEDevice.gatt.connected) {
                BLEDevice.gatt.disconnect();
                console.log('切斷連線');
            }
        }
        console.log('出錯啦～' + error);
    }
}

function UIinit() {
    $('#connBtn').removeAttr("disabled");  // 啟用「連線」按鈕
    $('#battery > i').attr('class', 'las la-battery-empty');
    $('#battLevel').text('0');
    $('#controller').hide(500);
}

function onDisconnected(e) {
    console.log('藍牙連線斷了～');
    UIinit();
}

$("#connBtn").click(e => {
    if (!navigator.bluetooth) {
        console.log('你的瀏覽器不支援Web Bluetooth API，換一個吧～');
        return false;
    }

    onConnBtnClick();
});

async function sendMsg(msg) {
    if (!BLEDevice) {
        return;
    }

    if (BLEDevice.gatt.connected) {
        try {
            const uartChar = await UARTService.getCharacteristic(RX_UUID);
            let enc = new TextEncoder();
            uartChar.writeValue(
                enc.encode(msg)
            )
        } catch (err) {
            console.log('出錯啦～' + err);
        }

    } else {
        return;
    }
}

$('#forwardBtn').click(e => {
    console.log('前進');
    sendMsg('w');
});

$('#leftBtn').click(e => {
    console.log('左轉');
    sendMsg('a');
});

$('#rightBtn').click(e => {
    console.log('右轉');
    sendMsg('d');
});

$('#backwardBtn').click(e => {
    console.log('後退');
    sendMsg('x');
});

$('#stopBtn').click(e => {
    console.log('停止');
    sendMsg('s');
});