let my_bank_name = '';
let my_acc_number = '';
let name = '';
let acc_password = 0;
let money = 0;


// ------------입금------------
// 입금 선택
function deposit() {
    $('#topText').html('입금하실 은행을 선택해 주세요.');
    choiceBankDP();
}

// 은행 선택 창(part: 입금)
function choiceBankDP() {
    $('#button1').replaceWith('<button class="screen_btn" onclick="clickBankDP(\'로키\')" id="button1">로 키</button>');

    $('#button2').replaceWith('<button class="screen_btn" onclick="clickBankDP(\'농협\')" id="button2">농 협</button>');

    $('#button3').replaceWith('<button class="screen_btn" onclick="clickBankDP(\'신한\')" id="button3">신 한</button>');

    $('#button4').hide();

    $('#button5').replaceWith('<button class="screen_btn" onclick="clickBankDP(\'국민\')" id="button3">국 민</button>');

    $('#button6').replaceWith('<button class="screen_btn" onclick="clickBankDP(\'우리\')" id="button6">우 리</button>');

    $('#button7').replaceWith('<button class="screen_btn" onclick="clickBankDP(\'하나\')" id="button7">하 나</button>');
}

// 은행 선택 눌렀을 때(part: 입금)
function clickBankDP(bank) {
    my_bank_name = bank;
    $('#topText').html('고객 정보 입력');
    $('.screen_btn_all').remove();

    $('.screen_categories').append('<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">계좌번호</label>\n' +
        '    <input type="text" class="form-control" id="depositInputAcc" placeholder="Enter account number">\n' +
        '    <small id="emailHelp" class="form-text text-muted">입금하실 계좌번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputPassword1">금액</label>\n' +
        '    <input type="text" class="form-control" id="depositInputSum" placeholder="Enter deposit amount">\n' +
        '     <small id="emailHelp" class="form-text text-muted">입금액을 입력해주세요.</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="depositSubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="depositCancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#depositCancelBtn").css('margin-left', '10px');

    $('#depositSubmitBtn').click(function () {

        // 입력 받은 계좌번호, 입금액 변수 저장
        var acc_number = $('#depositInputAcc').val();
        money = $('#depositInputSum').val();

        // 계좌번호 입력 유효값 검사
        var getAcc = RegExp(/^[0-9]*$/);
        if (!getAcc.test($("#depositInputAcc").val()) || $("#depositInputAcc").val().length == 0) {
            alert("올바른 계좌번호를 입력하세요.");
            $("#depositInputAcc").val("");
            $("#depositInputAcc").focus();
            return false;
        }

        // 금액 입력 유효값 검사
        var getSum = RegExp(/^[0-9]*$/);
        if (!getSum.test($("#depositInputSum").val()) || $("#depositInputSum").val().length < 1 || $("#depositInputSum").val() <= 0) {
            alert("올바른 금액을 입력하세요.");
            $("#depositInputSum").val("");
            $("#depositInputSum").focus();
            return false;
        }

        if (Number(money) > 100000000000) {
            alert("입금 한도 초과!\n1회 입금 한도: 100,000,000,000원(일천억원)");
            $("#depositInputSum").val("");
            $("#depositInputSum").focus();
            return false;
        }

        $('#topText').html('입금 정보 확인');
        $('.form-class').remove(); // 화면 지우기

        $('.screen_categories').append('<ul class="list-group">\n' +
            '  <li class="list-group-item" id="ListBankName">은행 : </li>\n' +
            '  <li class="list-group-item" id="AccNum">계좌번호 : </li>\n' +
            '  <li class="list-group-item" id="depositSum">금액 : </li>\n' +
            '  <li class="list-group-item" id="depositInfo">OOO님께 000원을 입금합니다.</li>\n' +
            '</ul>');

        $("#ListBankName").text(my_bank_name + "은행");
        $("#AccNum").text("계좌번호 : " + acc_number);
        $("#depositSum").text("금액 : " + Number(money).toLocaleString() + "원");

        // db에 저장된 계좌번호와 일치 하는 이름 갖고 오기
        $.ajax({
            type: "GET",
            url: "/deposit",
            data: {
                bankName_give: my_bank_name,
                accNumber_give: acc_number
            },
            success: function (response) {
                let acc_name = response['accName'];
                $("#depositInfo").html('<span id="tempSpanName">acc_name</span>님에게 <span id="tempSpanMoney">money</span>을 입금합니다.');

                // 이름 정보에 밑줄
                $("#tempSpanName").text(acc_name);
                $("#tempSpanName").css('text-decoration', 'underline');

                // 금액 정보에 밑줄
                $("#tempSpanMoney").text(Number(money).toLocaleString() + '원');
                $("#tempSpanMoney").css('text-decoration', 'underline');
            },
            error: function () {
                alert('입력하신 계좌 정보가 없습니다!');
                btnCancel();
            }
        })

        $('.screen_categories').append('<div id="info"><br>위 정보가 맞으면 확인(입금), 틀리면 취소(초기화면)을 눌러주세요.<br><br></div>');
        $('.screen_categories').append('<button type="button" class="btn btn-success" id="depositOkBtn">확 인</button>')
        $('.screen_categories').append('<button type="button" class="btn btn-danger" id="depositCancelBtn" onclick="btnCancel()">취 소</button>')
        $("#depositCancelBtn").css('margin-left', '10px');

        // 입금 확인 버튼을 눌렀을 시
        $('#depositOkBtn').click(function () {
            $('#topText').html('거래명세표');

            // 입금 버튼을 눌렀을 때 거래 시각 저장
            var current_date = showTime();

            $.ajax({
                type: "POST",
                url: "/depositChangeInfo",
                data: {
                    accNumber_give: acc_number,
                    money_give: money
                },
                success: function (response) {
                    alert(response["msg"]);
                }
            })

            // 화면 지우기
            $('.list-group').remove();
            $('#info').html('<br>거래 감사합니다. 확인을 누르시면 초기 화면으로 돌아갑니다.<br><br>');

            $('#depositCancelBtn').hide();

            $('#depositOkBtn').replaceWith('<button type="button" class="btn btn-success" id="depositOkBtn" onclick="btnCancel()">확 인</button>');

            $('#info').before('<div id="statementOfAccount">\n' +
                '      <div id="container1">\n' +
                '        <div id="Rbank">\n' +
                '          <span class="nav_R">R</span><span class="nav_bank">bank</span>\n' +
                '        </div>\n' +
                '        거래명세표\n' +
                '      </div>\n' +
                '      [예금입금]\n' +
                '      <hr />\n' +
                '      <div class="comeSum">\n' +
                '        <span>요청금액</span>\n' +
                '        <span id="var1">20,000원</span>\n' +
                '      </div>\n' +
                '      <div>처리결과: 정상처리</div>\n' +
                '      <div class="right_box" id="var2">거래 후 잔액: 30000원</div>\n' +
                '      <div class="right_box">수수료: 0원</div>\n' +
                '      <hr />\n' +
                '      <div id="var3">거래일시: 2022.04.05 22:18</div>\n' +
                '      <div id="var4">계좌번호: 농협 74834203</div>\n' +
                '      <br />\n' +
                '      <p>정상 처리되었습니다. 감사합니다.</p>\n' +
                '    </div>')

            // 거래 명세표 값들 변수로 변경 작업
            $('#var1').text(Number(money).toLocaleString() + '원');

            var balance = afterBalance(my_bank_name, acc_number) + Number(money);

            $('#var2').text('거래 후 잔액: ' + Number(balance).toLocaleString() + '원');
            $('#var3').text('거래일시: ' + current_date);
            $('#var4').text('계좌번호: ' + my_bank_name + ' ' + acc_number);

            // 거래 명세표 화면 CSS 작업
            $('#statementOfAccount').css('border', '1px solid black').css('padding', '10px');
            $('#container1').css('display', 'flex').css('justify-content', 'space-between').css('align-items', 'center');
            $('hr').css('border-top', '2px dotted #bbb');
            $('.comeSum').css('display', 'flex').css('justify-content', 'space-between').css('font-size', '20px');
            $('.right_box').css('display', 'flex').css('flex-direction', 'row-reverse');
        })
    })
}


// ------------출금------------
// 출금 선택
function withdraw() {
    $('#topText').html('출금하실 은행을 선택해 주세요.');
    choiceBankWD();
}

// 은행 선택 창(part: 출금)
function choiceBankWD() {
    $('#button1').replaceWith('<button class="screen_btn" onclick="clickBankWD(\'로키\')" id="button1">로 키</button>');

    $('#button2').replaceWith('<button class="screen_btn" onclick="clickBankWD(\'농협\')" id="button2">농 협</button>');

    $('#button3').replaceWith('<button class="screen_btn" onclick="clickBankWD(\'신한\')" id="button3">신 한</button>');

    $('#button4').hide();

    $('#button5').replaceWith('<button class="screen_btn" onclick="clickBankWD(\'국민\')" id="button3">국 민</button>');

    $('#button6').replaceWith('<button class="screen_btn" onclick="clickBankWD(\'우리\')" id="button6">우 리</button>');

    $('#button7').replaceWith('<button class="screen_btn" onclick="clickBankWD(\'하나\')" id="button7">하 나</button>');
}

// 은행 선택 눌렀을 때(part: 출금)
function clickBankWD(bank) {
    my_bank_name = bank;
    $('#topText').html('고객 정보 입력');
    $('.screen_btn_all').remove();

    $('.screen_categories').append('<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">계좌번호</label>\n' +
        '    <input type="text" class="form-control" id="withdrawInputAcc" placeholder="Enter account number">\n' +
        '    <small class="form-text text-muted">출금하실 계좌번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">비밀번호</label>\n' +
        '    <input type="password" class="form-control" id="withdrawInputPassword" placeholder="Enter Password">\n' +
        '    <small class="form-text text-muted">출금하실 계좌 비밀번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="withdrawSubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="withdrawCancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#withdrawCancelBtn").css('margin-left', '10px');

    $('#withdrawSubmitBtn').click(function () {
        // 입력 받은 계좌번호, 비밀번호 변수 저장
        var acc_number = $('#withdrawInputAcc').val();
        acc_password = $('#withdrawInputPassword').val();

        // 계좌번호 입력 유효값 검사
        var getAcc = RegExp(/^[0-9]*$/);
        if (!getAcc.test($("#withdrawInputAcc").val()) || $("#withdrawInputAcc").val().length == 0) {
            alert("올바른 계좌번호를 입력하세요.");
            $("#withdrawInputAcc").val("");
            $("#withdrawInputAcc").focus();
            return false;
        }

        // 비밀번호 입력 유효값 검사
        var getPassword = RegExp(/^[0-9]*$/);
        if (!getPassword.test($("#withdrawInputPassword").val()) || $("#withdrawInputPassword").val().length == 0) {
            alert("올바른 비밀번호를 입력하세요.");
            $("#withdrawInputPassword").val("");
            $("#withdrawInputPassword").focus();
            return false;
        }

        var errorCheck = checkPasswordError(acc_number, acc_password);

        // 비밀번호 오류 횟수 카운트
        var errorCount = getErrorCount(my_bank_name, acc_number);

        if (errorCheck == 1) {
            alert(`계좌 비밀번호 오류 횟수 : ${errorCount}회\n5회 이상 오류시 계좌가 정지됩니다.`);
            $("#withdrawInputPassword").val("");
            $("#withdrawInputPassword").focus();
        } else if (errorCheck == -1) {
            alert('정지된 계좌입니다. 관리자에게 문의하세요!');
            btnCancel();
        } else {
            // error 횟수 초기화
            initError(acc_number);
            // 입력값과 db에 저장된 은행, 계좌번호, 비밀번호 일치 확인 후
            // 계좌 소유자의 이름[0], 현재 잔액[1] 배열 형태로 반환
            var client_info = checkAndReturn(my_bank_name, acc_number, acc_password);
            var client_name = client_info[0];
            var client_current_balance = client_info[1];

            $('#topText').html('출금액 입력');
            $('.form-class').remove(); // 화면 지우기

            $('.screen_categories').append('<form class="form-class">\n' +
                '  <div class="form-group">\n' +
                '    <label for="exampleInputEmail1" id="namePrint">OOO님 안녕하세요.</label><br>\n' +
                '    <label for="exampleInputEmail1" id="balancePrint">현재 잔액은 0원 입니다.</label>\n' +
                '  </div>\n' +
                '  <hr class="temphr"/>\n' +
                '</form>');

            $(".temphr").css('border-top', '1px dashed #bbb');

            $("#namePrint").text(client_name + "님 안녕하세요.");
            $("#balancePrint").html('현재 잔액은 <span id="spanBalance">client_current_balance</span> 입니다.');

            // 잔액 정보에 밑줄
            $("#spanBalance").text(Number(client_current_balance).toLocaleString() + "원").css('text-decoration', 'underline');

            $('.screen_categories').append('<form class="form-class">\n' +
                '  <div class="form-group">\n' +
                '    <label for="exampleInputEmail1">금액</label>\n' +
                '    <input type="text" class="form-control" id="withdrawInputSum" placeholder="Enter withdraw amount">\n' +
                '    <small class="form-text text-muted">출금액을 입력해주세요.</small>\n' +
                '  </div>\n' +
                '  <button type="button" class="btn btn-success" id="withdrawSubmitBtn">확인</button>\n' +
                '</form>');

            $('#withdrawSubmitBtn').click(function () {
                $('#topText').html('출금 정보 확인');

                // 금액 입력 유효값 검사
                var getSum = RegExp(/^[0-9]*$/);
                if (!getSum.test($("#withdrawInputSum").val()) || $("#withdrawInputSum").val().length == 0 || $("#withdrawInputSum").val() == 0) {
                    alert("올바른 금액을 입력하세요.");
                    $("#withdrawInputSum").val("");
                    $("#withdrawInputSum").focus();
                    return false;
                }

                money = $('#withdrawInputSum').val();

                if (client_current_balance < Number(money)) {
                    alert('잔액이 부족합니다.');
                    $("#withdrawInputSum").val("");
                    $("#withdrawInputSum").focus();
                    return false;
                }

                $('.form-class').remove(); // 화면 지우기

                $('.screen_categories').append('<ul class="list-group">\n' +
                    '  <li class="list-group-item" id="ListBankName">은행 : </li>\n' +
                    '  <li class="list-group-item" id="AccNum">계좌번호 : </li>\n' +
                    '  <li class="list-group-item" id="withdrawSum">출금액 : </li>\n' +
                    '  <li class="list-group-item" id="withdrawInfo">OOO님의 계좌에서 000원을 출금합니다.</li>\n' +
                    '</ul>');

                $("#ListBankName").text(my_bank_name + "은행");
                $("#AccNum").text("계좌번호 : " + acc_number);
                $("#withdrawSum").text("출금액 : " + Number(money).toLocaleString() + "원");

                $("#withdrawInfo").html('<span id="tempSpanName">client_name</span>님의 계좌에서 <span id="tempSpanMoney">money</span>을 출금합니다.');

                // 이름 정보에 밑줄
                $("#tempSpanName").text(client_name);
                $("#tempSpanName").css('text-decoration', 'underline');

                // 금액 정보에 밑줄
                $("#tempSpanMoney").text(Number(money).toLocaleString() + '원');
                $("#tempSpanMoney").css('text-decoration', 'underline');


                $('.screen_categories').append('<div id="info"><br>위 정보가 맞으면 확인(출금), 틀리면 취소(초기화면)을 눌러주세요.<br><br></div>');
                $('.screen_categories').append('<button type="button" class="btn btn-success" id="withdrawOkBtn">확 인</button>')
                $('.screen_categories').append('<button type="button" class="btn btn-danger" id="withdrawCancelBtn" onclick="btnCancel()">취 소</button>')
                $("#withdrawCancelBtn").css('margin-left', '10px');

                // 출금 확인 버튼을 눌렀을 시
                $('#withdrawOkBtn').click(function () {
                    $('#topText').html('거래명세표');

                    // 출금 버튼을 눌렀을 때 거래 시각 저장
                    var current_date = showTime();

                    // 서버로 계좌번호, 출금액 전달
                    $.ajax({
                        type: "POST",
                        url: "/withdrawChangeInfo",
                        data: {
                            accNumber_give: acc_number,
                            money_give: money
                        },
                        success: function (response) {
                            alert(response["msg"]);
                        }
                    })

                    // 화면 지우기
                    $('.list-group').remove();
                    $('#info').html('<br>거래 감사합니다. 확인을 누르시면 초기 화면으로 돌아갑니다.<br><br>');

                    $('#withdrawCancelBtn').hide();

                    $('#withdrawOkBtn').replaceWith('<button type="button" class="btn btn-success" id="withdrawOkBtn" onclick="btnCancel()">확 인</button>');

                    $('#info').before('<div id="statementOfAccount">\n' +
                        '      <div id="container1">\n' +
                        '        <div id="Rbank">\n' +
                        '          <span class="nav_R">R</span><span class="nav_bank">bank</span>\n' +
                        '        </div>\n' +
                        '        거래명세표\n' +
                        '      </div>\n' +
                        '      [예금출금]\n' +
                        '      <hr />\n' +
                        '      <div class="comeSum">\n' +
                        '        <span>요청금액</span>\n' +
                        '        <span id="var1">20,000원</span>\n' +
                        '      </div>\n' +
                        '      <div>처리결과: 정상처리</div>\n' +
                        '      <div class="right_box" id="var2">거래 후 잔액: 30000원</div>\n' +
                        '      <div class="right_box">수수료: 0원</div>\n' +
                        '      <hr />\n' +
                        '      <div id="var3">거래일시: 2022.04.05 22:18</div>\n' +
                        '      <div id="var4">계좌번호: 농협 74834203</div>\n' +
                        '      <br />\n' +
                        '      <p>정상 처리되었습니다. 감사합니다.</p>\n' +
                        '    </div>')

                    // 거래 명세표 값들 변수로 변경 작업
                    $('#var1').text(Number(money).toLocaleString() + '원');

                    var balance = afterBalance(my_bank_name, acc_number) - Number(money);

                    $('#var2').text('거래 후 잔액: ' + Number(balance).toLocaleString() + '원');
                    $('#var3').text('거래일시: ' + current_date);
                    $('#var4').text('계좌번호: ' + my_bank_name + ' ' + acc_number);

                    // 거래 명세표 화면 CSS 작업
                    $('#statementOfAccount').css('border', '1px solid black').css('padding', '10px');
                    $('#container1').css('display', 'flex').css('justify-content', 'space-between').css('align-items', 'center');
                    $('hr').css('border-top', '2px dotted #bbb');
                    $('.comeSum').css('display', 'flex').css('justify-content', 'space-between').css('font-size', '20px');
                    $('.right_box').css('display', 'flex').css('flex-direction', 'row-reverse');
                })
            })
        }
    })
}


// ------------송금------------
// 송금 선택
function remit() {
    $('#topText').html('송금인 은행 선택');
    choiceBankRM_my();
}

// 은행 선택 창(part: 송금_송금인)
function choiceBankRM_my() {
    $('#button1').replaceWith('<button class="screen_btn" onclick="clickBankRM_my(\'로키\')" id="button1">로 키</button>');

    $('#button2').replaceWith('<button class="screen_btn" onclick="clickBankRM_my(\'농협\')" id="button2">농 협</button>');

    $('#button3').replaceWith('<button class="screen_btn" onclick="clickBankRM_my(\'신한\')" id="button3">신 한</button>');

    $('#button4').hide();

    $('#button5').replaceWith('<button class="screen_btn" onclick="clickBankRM_my(\'국민\')" id="button3">국 민</button>');

    $('#button6').replaceWith('<button class="screen_btn" onclick="clickBankRM_my(\'우리\')" id="button6">우 리</button>');

    $('#button7').replaceWith('<button class="screen_btn" onclick="clickBankRM_my(\'하나\')" id="button7">하 나</button>');
}

// 은행 선택 눌렀을 때(part: 송금_송금인)
function clickBankRM_my(my_bank) {
    $('#topText').html('송금인 정보 입력');
    my_bank_name = my_bank;
    $('.screen_btn_all').remove();

    $('.screen_categories').append('<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">계좌번호</label>\n' +
        '    <input type="text" class="form-control" id="myInputAcc" placeholder="Enter account number">\n' +
        '    <small class="form-text text-muted">송금인 계좌번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">비밀번호</label>\n' +
        '    <input type="password" class="form-control" id="myInputPassword" placeholder="Enter Password">\n' +
        '    <small class="form-text text-muted">계좌 비밀번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="remitSubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="remitCancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#remitCancelBtn").css('margin-left', '10px');

    $('#remitSubmitBtn').click(function () {

        // 입력 받은 계좌번호, 비밀번호 변수 저장
        var acc_number = $('#myInputAcc').val();
        acc_password = $('#myInputPassword').val();

        // 계좌번호 입력 유효값 검사
        var getAcc = RegExp(/^[0-9]*$/);
        if (!getAcc.test($("#myInputAcc").val()) || $("#myInputAcc").val().length == 0) {
            alert("올바른 계좌번호를 입력하세요.");
            $("#myInputAcc").val("");
            $("#myInputAcc").focus();
            return false;
        }

        // 비밀번호 입력 유효값 검사
        var getPassword = RegExp(/^[0-9]*$/);
        if (!getPassword.test($("#myInputPassword").val())) {
            alert("올바른 비밀번호를 입력하세요.");
            $("#myInputPassword").val("");
            $("#myInputPassword").focus();
            return false;
        }

        // 입력값과 db에 저장된 은행, 계좌번호, 비밀번호 일치 확인 후
        // 계좌 소유자의 이름[0], 현재 잔액[1] 배열 형태로 반환
        var my_client_info = checkAndReturn(my_bank_name, acc_number, acc_password);
        var my_client_name = my_client_info[0];
        var my_client_current_balance = my_client_info[1];

        $('#topText').html('송금액 입력');
        $('.form-class').remove(); // 화면 지우기

        $('.screen_categories').append('<form class="form-class">\n' +
            '  <div class="form-group">\n' +
            '    <label for="exampleInputEmail1" id="namePrint">OOO님 안녕하세요.</label><br>\n' +
            '    <label for="exampleInputEmail1" id="balancePrint">현재 잔액은 0원 입니다.</label>\n' +
            '  </div>\n' +
            '  <hr class="temphr"/>\n' +
            '</form>');

        $(".temphr").css('border-top', '1px dashed #bbb');

        $("#namePrint").text(my_client_name + "님 안녕하세요.");
        $("#balancePrint").html('현재 잔액은 <span id="spanBalance">my_client_current_balance</span> 입니다.');

        // 잔액 정보에 밑줄
        $("#spanBalance").text(Number(my_client_current_balance).toLocaleString() + "원").css('text-decoration', 'underline');

        $('.screen_categories').append('<form class="form-class">\n' +
            '  <div class="form-group">\n' +
            '    <label for="exampleInputEmail1">금액</label>\n' +
            '    <input type="text" class="form-control" id="remitInputSum" placeholder="Enter remit amount">\n' +
            '    <small class="form-text text-muted">송금액을 입력해주세요.</small>\n' +
            '  </div>\n' +
            '  <button type="button" class="btn btn-success" id="remitSubmitBtn">확인</button>\n' +
            '</form>');

        $('#remitSubmitBtn').click(function () {
            // 금액 입력 유효값 검사
            var getSum = RegExp(/^[0-9]*$/);
            if (!getSum.test($("#remitInputSum").val()) || $("#remitInputSum").val().length == 0 || $("#remitInputSum").val() == 0) {
                alert("올바른 금액을 입력하세요.");
                $("#remitInputSum").val("");
                $("#remitInputSum").focus();
                return false;
            }

            $('#topText').html('수취인 정보 입력');

            money = $('#remitInputSum').val(); // 송금액 입력값 저장

            if (my_client_current_balance < Number(money)) {
                alert('잔액이 부족합니다.');
                $("#remitInputSum").val("");
                $("#remitInputSum").focus();
                return false;
            }

            $('.form-class').remove(); // 화면 지우기

            $('.screen_categories').append(
                '<form class="form-class">\n' +
                '  <div class="form-group">\n' +
                '    <label for="exampleInputEmail1">은행</label>\n' +
                '    <select class="custom-select" id="BankSelect">\n' +
                '       <option value="" selected>Choose...</option>\n' +
                '       <option value="로키">로키은행</option>\n' +
                '       <option value="농협">농협은행</option>\n' +
                '       <option value="신한">신한은행</option>\n' +
                '       <option value="국민">국민은행</option>\n' +
                '       <option value="우리">우리은행</option>\n' +
                '       <option value="하나">하나은행</option>\n' +
                '   </select>\n' +
                '   <small class="form-text text-muted">송금 받으실 분의 은행을 선택해주세요.</small>\n' +
                '  </div>\n' +
                '  <div class="form-group">\n' +
                '    <label for="exampleInputEmail1">계좌번호</label>\n' +
                '    <input type="text" class="form-control" id="yourInputAcc" placeholder="Enter account number">\n' +
                '    <small class="form-text text-muted">송금 받으실 분의 계좌번호를 입력해주세요.</small>\n' +
                '  </div>\n' +
                '   <button type="button" class="btn btn-success" id="yourSubmitBtn">확 인</button>\n' +
                '   <button type="button" class="btn btn-danger" id="yourCancelBtn" onclick="btnCancel()">취 소</button>\n' +
                '</form>'
            );

            $("#yourCancelBtn").css('margin-left', '10px');

            $('#yourSubmitBtn').click(function () {

                // 입력 받은 수취인 은행, 계좌번호 저장
                var your_bank_name = $('#BankSelect').val();
                var your_acc_number = $('#yourInputAcc').val();

                // 수취인 은행, 계좌번호가 DB에 저장된 값과 일치한지 확인
                $.ajax({
                    type: "GET",
                    url: "/checkRemit",
                    data: {
                        myAccNumber_give: acc_number,
                        yourAccNumber_give: your_acc_number,
                        yourBankName_give: your_bank_name
                    },
                    success: function (response) {
                        var myAccNumber = response[0]['my_acc_number'];
                        var yourAccNumber = response[1]['your_acc_number'];
                        var yourName = response[2]['your_name'];

                        // 송금인과 수취인 계좌가 같은 경우
                        if (myAccNumber == yourAccNumber) {
                            alert('본인에게는 송금할 수 없습니다!');
                            $("#yourInputAcc").val("");
                            $("#yourInputAcc").focus();
                        }

                        // 동일 은행 송금일 경우
                        else if (my_bank_name == your_bank_name) {
                            var commission = 0;
                            $('#topText').html('송금 정보 확인');
                            $('.form-class').remove(); // 화면 지우기

                            $('.screen_categories').append('<form class="form-class">\n' +
                                '  <div class="form-group">\n' +
                                '    <label for="exampleInputEmail1">동일은행 송금입니다.</label><br>\n' +
                                '    <label for="exampleInputEmail1" id="commissionPrint">수수료 0원이 부과됩니다.</label>\n' +
                                '  </div>\n' +
                                '  <hr class="temphr"/>\n' +
                                '</form>');

                            $(".temphr").css('border-top', '1px dashed #bbb');

                            $("#commissionPrint").html('수수료 <span id="spanCommission">commission원</span>이 부과됩니다.');

                            // 수수료 정보에 밑줄
                            $("#spanCommission").text(commission + "원").css('text-decoration', 'underline');


                            $('.screen_categories').append('<br class="tempbr"><div class="remitInfo">\n' +
                                '      <div id="remitter">\n' +
                                '        <div class="order1">송금인</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order2">이름: 임록희</div>\n' +
                                '        <hr class="remitHr">\n' +
                                '        <div class="order3">계좌번호: 국민 52341267</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order4">송금액: 10000원</div>\n' +
                                '      </div>\n' +
                                '\n' +
                                '      <div id="addressee">\n' +
                                '        <div class="order1">수취인</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order2">이름: 윤지상</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order3">계좌번호: 농협 12741267</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order4">동일은행(수수료: 0원)</div>\n' +
                                '      </div>\n' +
                                '    </div>');

                            $("#remitter").find(".order2").text("이름: " + my_client_name);
                            $("#remitter").find(".order3").text("계좌번호: " + my_bank_name + " " + myAccNumber);
                            $("#remitter").find(".order4").text("송금액: " + Number(money).toLocaleString() + "원");
                            $("#addressee").find(".order2").text("이름: " + yourName);
                            $("#addressee").find(".order3").text("계좌번호: " + your_bank_name + " " + your_acc_number);

                            // CSS 작업
                            $('.remitInfo').css('display', 'flex');
                            $('#remitter').css('border', '1px solid black').css('width', '200px');
                            $('#addressee').css('border', '1px solid black').css('margin-left', '40px').css('width', '200px');
                            $('.order1').css('display', 'flex').css('justify-content', 'center').css('margin-top', '8px');
                            $('.order2, .order3, .order4').css('padding-left', '10px');
                            $('.order4').css('margin-bottom', '8px');
                            $('.remitHr').css('margin-top', '8px').css('margin-bottom', '8px');

                            $('.screen_categories').append('<div id="info"><br>위 정보가 맞으면 확인(이체), 틀리면 취소(초기화면)을 눌러주세요.<br><br></div>');
                            $('.screen_categories').append('<button type="button" class="btn btn-success" id="remitOkBtn">확 인</button>')
                            $('.screen_categories').append('<button type="button" class="btn btn-danger" id="remitCancelBtn" onclick="btnCancel()">취 소</button>')
                            $("#remitCancelBtn").css('margin-left', '10px');

                            // 송금 확인 버튼을 눌렀을 시
                            $('#remitOkBtn').click(function () {
                                $('#topText').html('거래명세표');

                                // 송금 버튼을 눌렀을 때 거래 시각 저장
                                var current_date = showTime();

                                // 서버로 계좌번호, 은행, 송금액 전달
                                $.ajax({
                                    type: "POST",
                                    url: "/remitChangeInfo",
                                    data: {
                                        myAccNumber_give: myAccNumber,
                                        yourAccNumber_give: yourAccNumber,
                                        myBankName_give: my_bank_name,
                                        yourBankName_give: your_bank_name,
                                        money_give: money
                                    },
                                    success: function (response) {
                                        alert(response["msg"]);
                                    }
                                })

                                // 화면 지우기
                                $('.form-class').remove();
                                $('.remitInfo').remove();
                                $('#remitOkBtn').remove();
                                $('.tempbr').remove();

                                $('#info').html('<br>거래 감사합니다. 확인을 누르시면 초기 화면으로 돌아갑니다.<br><br>');

                                $('#remitCancelBtn').hide();

                                $('#remitCancelBtn').replaceWith('<button type="button" class="btn btn-success" id="withdrawOkBtn" onclick="btnCancel()">확 인</button>');

                                $('#info').before('<div id="statementOfAccount">\n' +
                                    '      <div id="container1">\n' +
                                    '        <div id="Rbank">\n' +
                                    '          <span class="nav_R">R</span><span class="nav_bank">bank</span>\n' +
                                    '        </div>\n' +
                                    '        거래명세표\n' +
                                    '      </div>\n' +
                                    '      [계좌이체]\n' +
                                    '      <hr />\n' +
                                    '      <div class="comeSum">\n' +
                                    '        <span>요청금액</span>\n' +
                                    '        <span id="var1">20,000원</span>\n' +
                                    '      </div>\n' +
                                    '      <div>처리결과: 정상처리</div>\n' +
                                    '      <div class="right_box" id="var2">거래 후 잔액: 30000원</div>\n' +
                                    '      <div class="right_box">수수료: 0원</div>\n' +
                                    '      <hr />\n' +
                                    '      <div id="var3">거래일시: 2022.04.05 22:18</div>\n' +
                                    '      <div id="var4">송금인 계좌번호: 농협 74834203</div>\n' +
                                    '      <div id="var5">수취인 계좌번호: 국민 94822043</div>\n' +

                                    '      <br />\n' +
                                    '      <p>정상 처리되었습니다. 감사합니다.</p>\n' +
                                    '    </div>')

                                // 거래 명세표 값들 변수로 변경 작업
                                $('#var1').text(Number(money).toLocaleString() + '원');

                                var balance = afterBalance(my_bank_name, acc_number) - Number(money);

                                $('#var2').text('거래 후 잔액: ' + Number(balance).toLocaleString() + '원');
                                $('#var3').text('거래일시: ' + current_date);
                                $('#var4').text('송금인 계좌번호: ' + my_bank_name + ' ' + acc_number);
                                $('#var5').text('수취인 계좌번호: ' + your_bank_name + ' ' + yourAccNumber);

                                // 거래 명세표 화면 CSS 작업
                                $('#statementOfAccount').css('border', '1px solid black').css('padding', '10px');
                                $('#container1').css('display', 'flex').css('justify-content', 'space-between').css('align-items', 'center');
                                $('hr').css('border-top', '2px dotted #bbb');
                                $('.comeSum').css('display', 'flex').css('justify-content', 'space-between').css('font-size', '20px');
                                $('.right_box').css('display', 'flex').css('flex-direction', 'row-reverse');
                            })
                        }
                        // 타은행 송금일 경우
                        else {
                            var commission = 500;
                            $('#topText').html('송금 정보 확인');
                            $('.form-class').remove(); // 화면 지우기

                            $('.screen_categories').append('<form class="form-class">\n' +
                                '  <div class="form-group">\n' +
                                '    <label for="exampleInputEmail1">타은행 송금입니다.</label><br>\n' +
                                '    <label for="exampleInputEmail1" id="commissionPrint">수수료 000원이 부과됩니다.</label>\n' +
                                '  </div>\n' +
                                '  <hr class="temphr"/>\n' +
                                '</form>');

                            $(".temphr").css('border-top', '1px dashed #bbb');

                            $("#commissionPrint").html('수수료 <span id="spanCommission">commission원</span>이 부과됩니다.');

                            // 수수료 정보에 밑줄
                            $("#spanCommission").text(commission + "원").css('text-decoration', 'underline');


                            $('.screen_categories').append('<br class="tempbr"><div class="remitInfo">\n' +
                                '      <div id="remitter">\n' +
                                '        <div class="order1">송금인</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order2">이름: 임록희</div>\n' +
                                '        <hr class="remitHr">\n' +
                                '        <div class="order3">계좌번호: 국민 52341267</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order4">송금액: 10000원</div>\n' +
                                '      </div>\n' +
                                '\n' +
                                '      <div id="addressee">\n' +
                                '        <div class="order1">수취인</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order2">이름: 윤지상</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order3">계좌번호: 농협 12741267</div>\n' +
                                '        <hr class="remitHr"/>\n' +
                                '        <div class="order4">타은행(수수료: 500원)</div>\n' +
                                '      </div>\n' +
                                '    </div>');

                            $("#remitter").find(".order2").text("이름: " + my_client_name);
                            $("#remitter").find(".order3").text("계좌번호: " + my_bank_name + " " + myAccNumber);
                            $("#remitter").find(".order4").text("송금액: " + Number(money).toLocaleString() + "원");
                            $("#addressee").find(".order2").text("이름: " + yourName);
                            $("#addressee").find(".order3").text("계좌번호: " + your_bank_name + " " + your_acc_number);

                            // CSS 작업
                            $('.remitInfo').css('display', 'flex');
                            $('#remitter').css('border', '1px solid black').css('width', '200px');
                            $('#addressee').css('border', '1px solid black').css('margin-left', '40px').css('width', '200px');
                            $('.order1').css('display', 'flex').css('justify-content', 'center').css('margin-top', '8px');
                            $('.order2, .order3, .order4').css('padding-left', '10px');
                            $('.order4').css('margin-bottom', '8px');
                            $('.remitHr').css('margin-top', '8px').css('margin-bottom', '8px');

                            $('.screen_categories').append('<div id="info"><br>위 정보가 맞으면 확인(이체), 틀리면 취소(초기화면)을 눌러주세요.<br><br></div>');
                            $('.screen_categories').append('<button type="button" class="btn btn-success" id="remitOkBtn">확 인</button>')
                            $('.screen_categories').append('<button type="button" class="btn btn-danger" id="remitCancelBtn" onclick="btnCancel()">취 소</button>')
                            $("#remitCancelBtn").css('margin-left', '10px');

                            // 송금 확인 버튼을 눌렀을 시
                            $('#remitOkBtn').click(function () {
                                $('#topText').html('거래명세표');

                                if (my_client_current_balance < (Number(money) + commission)) {
                                    alert('잔액을 확인해주세요.\n{message: 수수료 부족}');
                                    btnCancel();
                                } else {
                                    // 송금 버튼을 눌렀을 때 거래 시각 저장
                                    var current_date = showTime();

                                    // 서버로 계좌번호, 은행, 송금액 전달
                                    $.ajax({
                                        type: "POST",
                                        url: "/remitChangeInfo",
                                        data: {
                                            myAccNumber_give: myAccNumber,
                                            yourAccNumber_give: yourAccNumber,
                                            myBankName_give: my_bank_name,
                                            yourBankName_give: your_bank_name,
                                            money_give: money
                                        },
                                        success: function (response) {
                                            alert(response["msg"]);
                                        }
                                    })

                                    // 화면 지우기
                                    $('.form-class').remove();
                                    $('.remitInfo').remove();
                                    $('#remitOkBtn').remove();
                                    $('.tempbr').remove();

                                    $('#info').html('<br>거래 감사합니다. 확인을 누르시면 초기 화면으로 돌아갑니다.<br><br>');

                                    $('#remitCancelBtn').hide();

                                    $('#remitCancelBtn').replaceWith('<button type="button" class="btn btn-success" id="withdrawOkBtn" onclick="btnCancel()">확 인</button>');

                                    $('#info').before('<div id="statementOfAccount">\n' +
                                        '      <div id="container1">\n' +
                                        '        <div id="Rbank">\n' +
                                        '          <span class="nav_R">R</span><span class="nav_bank">bank</span>\n' +
                                        '        </div>\n' +
                                        '        거래명세표\n' +
                                        '      </div>\n' +
                                        '      [계좌이체]\n' +
                                        '      <hr />\n' +
                                        '      <div class="comeSum">\n' +
                                        '        <span>요청금액</span>\n' +
                                        '        <span id="var1">20,000원</span>\n' +
                                        '      </div>\n' +
                                        '      <div>처리결과: 정상처리</div>\n' +
                                        '      <div class="right_box" id="var2">거래 후 잔액: 30000원</div>\n' +
                                        '      <div class="right_box">수수료: 500원</div>\n' +
                                        '      <hr />\n' +
                                        '      <div id="var3">거래일시: 2022.04.05 22:18</div>\n' +
                                        '      <div id="var4">송금인 계좌번호: 농협 74834203</div>\n' +
                                        '      <div id="var5">수취인 계좌번호: 국민 94822043</div>\n' +

                                        '      <br />\n' +
                                        '      <p>정상 처리되었습니다. 감사합니다.</p>\n' +
                                        '    </div>')

                                    // 거래 명세표 값들 변수로 변경 작업
                                    $('#var1').text(Number(money).toLocaleString() + '원');

                                    var balance = afterBalance(my_bank_name, acc_number) - Number(money) - 500;

                                    $('#var2').text('거래 후 잔액: ' + Number(balance).toLocaleString() + '원');
                                    $('#var3').text('거래일시: ' + current_date);
                                    $('#var4').text('송금인 계좌번호: ' + my_bank_name + ' ' + acc_number);
                                    $('#var5').text('수취인 계좌번호: ' + your_bank_name + ' ' + yourAccNumber);

                                    // 거래 명세표 화면 CSS 작업
                                    $('#statementOfAccount').css('border', '1px solid black').css('padding', '10px');
                                    $('#container1').css('display', 'flex').css('justify-content', 'space-between').css('align-items', 'center');
                                    $('hr').css('border-top', '2px dotted #bbb');
                                    $('.comeSum').css('display', 'flex').css('justify-content', 'space-between').css('font-size', '20px');
                                    $('.right_box').css('display', 'flex').css('flex-direction', 'row-reverse');
                                }


                            })
                        }
                    },
                    error: function (e) {
                        alert("수취인의 계좌정보가 없습니다!\n은행과 계좌번호를 다시 확인해주세요.");
                        $("#yourInputAcc").val("");
                        $("#yourInputAcc").focus();

                    }
                });

            })
        })
    })
}


// ------------계좌생성------------
// 계좌생성 선택
function createAccount() {
    $('#topText').html('만드시는 은행을 선택해 주세요.');

    choiceBankCRA();
}

// 은행 선택 창(part: 계좌 생성)
function choiceBankCRA() {
    $('#button1').replaceWith('<button class="screen_btn" onclick="clickBankCRA(\'로키\')" id="button1">로 키</button>');

    $('#button2').replaceWith('<button class="screen_btn" onclick="clickBankCRA(\'농협\')" id="button2">농 협</button>');

    $('#button3').replaceWith('<button class="screen_btn" onclick="clickBankCRA(\'신한\')" id="button3">신 한</button>');

    $('#button4').hide();

    $('#button5').replaceWith('<button class="screen_btn" onclick="clickBankCRA(\'국민\')" id="button3">국 민</button>');

    $('#button6').replaceWith('<button class="screen_btn" onclick="clickBankCRA(\'우리\')" id="button6">우 리</button>');

    $('#button7').replaceWith('<button class="screen_btn" onclick="clickBankCRA(\'하나\')" id="button7">하 나</button>');

}

// 은행 선택 눌렀을 때(part: 계좌 생성)
function clickBankCRA(bank) {
    my_bank_name = bank;
    $('#topText').html('고객 정보 입력');
    $('.screen_btn_all').remove();

    $('.screen_categories').append('<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">Name</label>\n' +
        '    <input type="text" class="form-control" id="createAccInputName" placeholder="Enter name">\n' +
        '    <small id="emailHelp" class="form-text text-muted">이름(한글)을 입력해주세요.</small>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputPassword1">Password</label>\n' +
        '    <input type="password" class="form-control" id="createAccInputPassword" placeholder="Enter password">\n' +
        '     <small id="emailHelp" class="form-text text-muted">사용하실 계좌 비밀번호를 입력해주세요.<br>4자 숫자 사용</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="createAccSubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="createAccCancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#createAccCancelBtn").css('margin-left', '10px');

    $('#createAccSubmitBtn').click(function () {

        // 입력값 유효성 검사
        var check = checkException();
        if (check == false) {
            return;
        }

        // 입력 받은 이름, 비밀번호 변수 저장
        name = $('#createAccInputName').val();
        acc_password = $('#createAccInputPassword').val();

        // 랜덤 생성된 계좌번호 8자리 변수 저장
        my_acc_number = generateAccNum(10000000, 99999999); // 계좌번호 랜덤생성 함수

        $('#topText').html('고객 정보 확인');
        $('.form-class').remove(); // 화면 지우기

        $('.screen_categories').append('<ul class="list-group">\n' +
            '  <li class="list-group-item" id="ListBankName">은행 : </li>\n' +
            '  <li class="list-group-item" id="newAccNum">신규 계좌번호</li>\n' +
            '  <li class="list-group-item" id="newName">이름</li>\n' +
            '  <li class="list-group-item" id="newPassword">비밀번호</li>\n' +
            '</ul>');

        $("#ListBankName").text(my_bank_name + "은행");
        $("#newAccNum").text("신규 계좌번호 : " + my_acc_number);
        $("#newName").text("이름 : " + name);
        $("#newPassword").text("비밀번호 : " + acc_password);

        $('.screen_categories').append('<div><br>위 정보가 맞으면 확인(계좌생성), 틀리면 취소(초기화면)을 눌러주세요.<br><br></div>');
        $('.screen_categories').append('<button type="button" class="btn btn-success" id="createAccOkBtn">확 인</button>')
        $('.screen_categories').append('<button type="button" class="btn btn-danger" id="createAccCancelBtn" onclick="btnCancel()">취 소</button>')
        $("#createAccCancelBtn").css('margin-left', '10px');

        $('#createAccOkBtn').click(function () {

            $.ajax({
                type: "POST",
                url: "/createAccount",
                data: {
                    bankName_give: my_bank_name,
                    accNumber_give: my_acc_number,
                    name_give: name,
                    password_give: acc_password
                },
                success: function (response) {
                    alert(response["msg"]);
                    btnCancel();
                }
            })
        })
    })
}

// 유효성 검사(계좌생성 part)
function checkException() {
    var getName = RegExp(/^[가-힣]+$/);
    var getPassword = RegExp(/^[0-9]*$/);

    //이름 유효성 검사
    if (!getName.test($("#createAccInputName").val())) {
        alert("정확한 이름(한글)을 입력해주세요!");
        $("#createAccInputName").val("");
        $("#createAccInputName").focus();
        return false;
    }

    // 이름 입력이 2 ~ 5자 사이가 아닐 경우
    if ($('#createAccInputName').val().length < 2 || $('#createAccInputName').val().length > 5) {
        alert("정확한 이름을 입력해주세요.");
        $("#createAccInputName").focus();
        return false;
    }

    //비밀번호 입력이 숫자가 아닐 경우
    if (!getPassword.test($("#createAccInputPassword").val())) {
        alert("숫자(4자리)를 입력하세요.");
        $("#createAccInputPassword").val("");
        $("#createAccInputPassword").focus();
        return false;
    }

    // 비밀번호 입력이 4자가 아닐 경우
    if ($('#createAccInputPassword').val().length != 4) {
        alert("4자리로 입력해주세요.");
        $("#createAccInputPassword").val("");
        $("#createAccInputPassword").focus();
        return false;
    }
}


// ------------비밀번호 변경------------
// 비밀번호 변경 선택
function changePassword() {

    $('#topText').html('비밀번호 변경');
    $('.screen_btn_all').remove();

    $('.screen_categories').append(
        '<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">은행</label>\n' +
        '    <select class="custom-select" id="inputBankSelect">\n' +
        '       <option value="" selected>Choose...</option>\n' +
        '       <option value="로키">로키은행</option>\n' +
        '       <option value="농협">농협은행</option>\n' +
        '       <option value="신한">신한은행</option>\n' +
        '       <option value="국민">국민은행</option>\n' +
        '       <option value="우리">우리은행</option>\n' +
        '       <option value="하나">하나은행</option>\n' +
        '   </select>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">계좌번호</label>\n' +
        '    <input type="text" class="form-control" id="InputAcc" placeholder="Enter account number">\n' +
        '    <small class="form-text text-muted">변경하실 계좌번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">비밀번호</label>\n' +
        '    <input type="password" class="form-control" id="InputPassword" placeholder="Enter Password">\n' +
        '    <small class="form-text text-muted">계좌 비밀번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="SubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="CancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#CancelBtn").css('margin-left', '10px');

    $('#SubmitBtn').click(function () {
            my_bank_name = $('#inputBankSelect').val();

            if (my_bank_name.length == 0) {
                alert("은행을 선택해주세요.");
                $("#inputBankSelect").val("");
                $("#inputBankSelect").focus();
                return false;
            }

            // 입력 받은 계좌번호, 비밀번호 변수 저장
            var acc_number = $('#InputAcc').val();
            acc_password = $('#InputPassword').val();

            // 계좌번호 입력 유효값 검사
            var getAcc = RegExp(/^[0-9]*$/);
            if (!getAcc.test($("#InputAcc").val()) || $("#InputAcc").val().length == 0) {
                alert("올바른 계좌번호를 입력하세요.");
                $("#InputAcc").val("");
                $("#InputAcc").focus();
                return false;
            }

            // 비밀번호 입력 유효값 검사
            var getPassword = RegExp(/^[0-9]*$/);
            if (!getPassword.test($("#InputPassword").val()) || $("#InputPassword").val().length == 0) {
                alert("올바른 비밀번호를 입력하세요.");
                $("#InputPassword").val("");
                $("#InputPassword").focus();
                return false;
            }

            var errorCheck = checkPasswordError(acc_number, acc_password);

            // 비밀번호 오류 횟수 카운트
            var errorCount = getErrorCount(my_bank_name, acc_number);

            if (errorCheck == 1) {
                alert(`계좌 비밀번호 오류 횟수 : ${errorCount}회\n5회 이상 오류시 계좌가 정지됩니다.`);
                $("#InputPassword").val("");
                $("#InputPassword").focus();
            } else if (errorCheck == -1) {
                alert('정지된 계좌입니다. 관리자에게 문의하세요!');
                btnCancel();
            } else {
                // error 횟수 초기화
                initError(acc_number);
                // 입력값과 db에 저장된 은행, 계좌번호, 비밀번호 일치 확인 후
                // 계좌 소유자의 이름[0], 현재 잔액[1] 배열 형태로 반환
                var client_info = checkAndReturn(my_bank_name, acc_number, acc_password);
                var client_name = client_info[0];

                $('.form-class').remove(); // 화면 지우기

                $('.screen_categories').append('<form class="form-class">\n' +
                    '  <div class="form-group">\n' +
                    '    <label for="exampleInputEmail1" id="namePrint">OOO님 안녕하세요.</label><br>\n' +
                    '    <label for="exampleInputEmail1">계좌 비밀번호 변경 화면입니다.</label>\n' +
                    '  </div>\n' +
                    '  <hr class="temphr"/>\n' +
                    '</form>');

                $(".temphr").css('border-top', '1px dashed #bbb');

                $("#namePrint").text(client_name + "님 안녕하세요.");

                $('.screen_categories').append('<form class="form-class">\n' +
                    '  <div class="form-group">\n' +
                    '    <label for="exampleInputEmail1">비밀번호</label>\n' +
                    '    <input type="password" class="form-control" id="newInputPassword" placeholder="Enter new password">\n' +
                    '    <small class="form-text text-muted">변경하실 비밀번호를 입력해주세요.<br>4자 숫자 사용</small>\n' +
                    '  </div>\n' +
                    '   <button type="button" class="btn btn-success" id="SubmitBtnCP">확 인</button>\n' +
                    '   <button type="button" class="btn btn-danger" id="CancelBtnCP" onclick="btnCancel()">취 소</button>\n' +
                    '</form>'
                );

                $("#CancelBtnCP").css('margin-left', '10px');

                // 비밀번호 변경 확인 버튼을 눌렀을 시
                $('#SubmitBtnCP').click(function () {

                    // 비밀번호 입력 유효값 검사
                    getPassword = RegExp(/^[0-9]*$/);
                    if (!getPassword.test($("#newInputPassword").val()) || $("#newInputPassword").val().length == 0) {
                        alert("숫자(4자리)를 입력하세요.");
                        $("#newInputPassword").val("");
                        $("#newInputPassword").focus();
                        return false;
                    }

                    // 비밀번호 입력이 4자가 아닐 경우
                    if ($('#newInputPassword').val().length != 4) {
                        alert("4자리로 입력해주세요.");
                        $("#newInputPassword").val("");
                        $("#newInputPassword").focus();
                        return false;
                    }

                    var new_password = $('#newInputPassword').val();

                    // 비밀번호 변경 버튼을 눌렀을 때 시각 저장
                    var current_date = showTime();

                    // 변경 비밀번호 DB에 반영
                    $.ajax({
                        type: "POST",
                        url: "/ChangeAccPassword",
                        data: {
                            accNumber_give: acc_number,
                            password_give: new_password
                        },
                        success: function (response) {
                            alert(response["msg"]);
                            // 새로 입력한 비밀번호가 기존 비밀번호와 같을 경우
                            if (response['result'] == 'false') {
                                $("#newInputPassword").val("");
                                $("#newInputPassword").focus();
                            } else {
                                $('.form-class').remove(); // 화면 지우기

                                $('.screen_categories').append('<ul class="list-group">\n' +
                                    '  <li class="list-group-item" id="ListBankName">은행 : </li>\n' +
                                    '  <li class="list-group-item" id="name">이름 : </li>\n' +
                                    '  <li class="list-group-item" id="accNum">계좌번호 : </li>\n' +
                                    '  <li class="list-group-item" id="password">변경 비밀번호 : </li>\n' +
                                    '  <p></p>\n' +
                                    '  <li id="checkDate">변경일시 : </li>\n' +
                                    '</ul>');

                                $('.screen_categories').append('<br>확인을 누르시면 초기 화면으로 돌아갑니다.<br><br>');
                                $('.screen_categories').append('<button type="button" class="btn btn-success" onclick="btnCancel()" id="#SubmitBtn">확 인</button>');

                                $("#ListBankName").text(my_bank_name + "은행");
                                $("#name").text("이름 : " + client_name);
                                $("#accNum").text("계좌번호 : " + acc_number);
                                $("#password").text("변경 비밀번호 : " + new_password);
                                $("#checkDate").text("변경일시 : " + current_date).css('display', 'flex').css('flex-direction', 'row-reverse');
                            }
                        }
                    })
                })
            }
        }
    )
}


// ------------예금조회------------

// 은행 선택 창(part: 예금조회)
function choiceBankBC() {
    $('#button1').replaceWith('<button class="screen_btn" onclick="clickBankBC(\'로키\')" id="button1">로 키</button>');

    $('#button2').replaceWith('<button class="screen_btn" onclick="clickBankBC(\'농협\')" id="button2">농 협</button>');

    $('#button3').replaceWith('<button class="screen_btn" onclick="clickBankBC(\'신한\')" id="button3">신 한</button>');

    $('#button4').hide();

    $('#button5').replaceWith('<button class="screen_btn" onclick="clickBankBC(\'국민\')" id="button3">국 민</button>');

    $('#button6').replaceWith('<button class="screen_btn" onclick="clickBankBC(\'우리\')" id="button6">우 리</button>');

    $('#button7').replaceWith('<button class="screen_btn" onclick="clickBankBC(\'하나\')" id="button7">하 나</button>');
}

// 예금조회 선택
function balanceCheck() {
    $('#topText').html('조회하실 은행을 선택해 주세요.');

    choiceBankBC();
}

// 은행 선택 눌렀을 때(part: 예금조회)
function clickBankBC(bank) {
    my_bank_name = bank;
    $('#topText').html('고객 정보 입력');
    $('.screen_btn_all').remove();

    $('.screen_categories').append('<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">계좌번호</label>\n' +
        '    <input type="text" class="form-control" id="InputAcc" placeholder="Enter account number">\n' +
        '    <small class="form-text text-muted">조회하실 계좌번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">비밀번호</label>\n' +
        '    <input type="password" class="form-control" id="InputPassword" placeholder="Enter Password">\n' +
        '    <small class="form-text text-muted">조회하실 계좌 비밀번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="SubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="CancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#CancelBtn").css('margin-left', '10px');

    $('#SubmitBtn').click(function () {
            $('#topText').html('예금조회');

            // 조회 버튼을 눌렀을 때 거래 시각 저장
            var current_date = showTime();

            // 입력 받은 계좌번호, 비밀번호 변수 저장
            var acc_number = $('#InputAcc').val();
            acc_password = $('#InputPassword').val();

            // 계좌번호 입력 유효값 검사
            var getAcc = RegExp(/^[0-9]*$/);
            if (!getAcc.test($("#InputAcc").val()) || $("#InputAcc").val().length == 0) {
                alert("올바른 계좌번호를 입력하세요.");
                $("#InputAcc").val("");
                $("#InputAcc").focus();
                return false;
            }

            // 비밀번호 입력 유효값 검사
            var getPassword = RegExp(/^[0-9]*$/);
            if (!getPassword.test($("#InputPassword").val()) || $("#InputPassword").val().length == 0) {
                alert("올바른 비밀번호를 입력하세요.");
                $("#InputPassword").val("");
                $("#InputPassword").focus();
                return false;
            }

            var errorCheck = checkPasswordError(acc_number, acc_password);

            // 비밀번호 오류 횟수 카운트
            var errorCount = getErrorCount(my_bank_name, acc_number);

            if (errorCheck == 1) {
                alert(`계좌 비밀번호 오류 횟수 : ${errorCount}회\n5회 이상 오류시 계좌가 정지됩니다.`);
                $("#InputPassword").val("");
                $("#InputPassword").focus();
            } else if (errorCheck == -1) {
                alert('정지된 계좌입니다. 관리자에게 문의하세요!');
                btnCancel();
            } else {
                // error 횟수 초기화
                initError(acc_number);
                // 입력값과 db에 저장된 은행, 계좌번호, 비밀번호 일치 확인 후
                // 계좌 소유자의 이름[0], 현재 잔액[1] 배열 형태로 반환
                var client_info = checkAndReturn(my_bank_name, acc_number, acc_password);
                var client_name = client_info[0];
                var client_current_balance = client_info[1];

                $('.form-class').remove(); // 화면 지우기

                $('.screen_categories').append('<ul class="list-group">\n' +
                    '  <li class="list-group-item" id="ListBankName">은행 : </li>\n' +
                    '  <li class="list-group-item" id="name">이름 : </li>\n' +
                    '  <li class="list-group-item" id="AccNum">계좌번호 : </li>\n' +
                    '  <li class="list-group-item" id="checkSum">잔액 : </li>\n' +
                    '  <p></p>\n' +
                    '  <li id="checkDate">조회일시 : </li>\n' +
                    '</ul>');

                $('.screen_categories').append('<br>확인을 누르시면 초기 화면으로 돌아갑니다.<br><br>');
                $('.screen_categories').append('<button type="button" class="btn btn-success" onclick="btnCancel()" id="#SubmitBtn">확 인</button>');

                $("#ListBankName").text(my_bank_name + "은행");
                $("#name").text("이름 : " + client_name);
                $("#AccNum").text("계좌번호 : " + acc_number);
                $("#checkSum").text("잔액 : " + Number(client_current_balance).toLocaleString() + "원").css('color', 'blue');
                $("#checkDate").text("조회일시 : " + current_date).css('display', 'flex').css('flex-direction', 'row-reverse');
            }
        }
    )
}


// ------------관리자------------
function administrator() {
    $('.screen_btn_all').remove();
    $('#topText').html('관리자 페이지');

    $('.screen_categories').append('<form class="form-class">\n' +
        '  <div class="form-group">\n' +
        '    <label for="exampleInputEmail1">비밀번호</label>\n' +
        '    <input type="password" class="form-control" id="InputPassword" placeholder="Enter Admin Password">\n' +
        '    <small class="form-text text-muted">관리자 비밀번호를 입력해주세요.</small>\n' +
        '  </div>\n' +
        '   <button type="button" class="btn btn-success" id="SubmitBtn">확 인</button>\n' +
        '   <button type="button" class="btn btn-danger" id="CancelBtn" onclick="btnCancel()">취 소</button>\n' +
        '</form>'
    );

    $("#CancelBtn").css('margin-left', '10px');

    $('#SubmitBtn').click(function () {
        // 비밀번호 입력 창에 값을 입력하지 않은 경우
        if ($("#InputPassword").val().length == 0) {
            alert("비밀번호를 입력해주세요!");
            $("#InputPassword").val("");
            $("#InputPassword").focus();
            return false;
        }

        var input_password = $('#InputPassword').val();
        var real_admin_password = checkAdminPassword(input_password);

        // 관리자 비밀번호가 다를 경우
        if (input_password != real_admin_password) {
            alert('관리자 비밀번호가 다릅니다!');
            $("#InputPassword").val("");
            $("#InputPassword").focus();
        } else {
            $('.form-class').remove();
            $('#topText').html('계좌 정지 해제(관리자)');

            $('.screen_categories').append('<form class="form-class">\n' +
                '  <div class="form-group">\n' +
                '    <label for="exampleInputEmail1">계좌번호</label>\n' +
                '    <input type="text" class="form-control" id="InputAcc" placeholder="Enter account number">\n' +
                '    <small class="form-text text-muted">error count를 초기화 할 계좌번호를 입력해주세요.</small>\n' +
                '  </div>\n' +
                '  <button type="button" class="btn btn-success" id="SubmitBtn">확인</button>\n' +
                '</form>');

            $('#SubmitBtn').click(function () {
                var input_acc_number = $('#InputAcc').val();
                var checkAcc = initError(input_acc_number);
                if (checkAcc == 1) {
                    alert("존재하지 않는 계좌입니다.")
                    $("#InputAcc").val("");
                    $("#InputAcc").focus();
                } else {
                    alert("해당 계좌의 error count가 정상 초기화 되었습니다.")
                    btnCancel();
                }
            })
        }
    })
}

// db에 저장된 관리자 비밀번호 가져오기
function checkAdminPassword(admin_password) {
    var password;
    $.ajax({
        type: "GET",
        url: "/getAdminPassword",
        async: false,
        data: {
            admin_password_give: admin_password,
        },
        success: function (response) {
            password = response['admin_password'];
        }
    });
    return password;
}


// ------------기타 함수들------------

// 두 값 사이의 정수 난수 생성 함수(랜덤 계좌 번호 생성)
function generateAccNum(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1)) + min; //최댓값도 포함, 최솟값도 포함
}

// 거래 후 잔액 가져오기
function afterBalance(bank_name, acc_number) {
    var newBalance;
    $.ajax({
        type: "GET",
        url: "/balance",
        async: false,
        data: {
            bankName_give: bank_name,
            accNumber_give: acc_number
        },
        success: function (response) {
            newBalance = response['balance'];
        },
        error: function (e) {
            alert('잔액을 가져오지 못했습니다.');
        }
    });
    return newBalance;
}

// 입력값과 db에 저장된 은행, 계좌번호, 비밀번호 일치 확인 후 계좌 소유자의 이름과, 현재 잔액 반환 함수
// 은행, 계좌번호, 비밀번호 값이 일치하지 않을 경우 안내 메시지 출력
function checkAndReturn(bank_name, acc_number, acc_password) {
    var name;
    var currentBalance;
    $.ajax({
        type: "GET",
        url: "/withdraw",
        async: false,
        data: {
            bankName_give: bank_name,
            accNumber_give: acc_number,
            accPassword_give: acc_password
        },
        success: function (response) {
            name = response[0]['name'];
            currentBalance = response[1]['balance'];
        },
        error: function () {
            alert('입력하신 계좌 정보가 없습니다!');
            btnCancel();
        }
    });
    return [name, currentBalance];
}

// 현재 시간 구하기
function showTime() {
    var today = new Date();

    var year = today.getFullYear();
    var month = ('0' + (today.getMonth() + 1)).slice(-2);
    var day = ('0' + today.getDate()).slice(-2);

    var dateString = year + '.' + month + '.' + day;

    var hours = ('0' + today.getHours()).slice(-2);
    var minutes = ('0' + today.getMinutes()).slice(-2);

    var timeString = hours + ':' + minutes;

    var showtime = dateString + ' ' + timeString;
    return showtime;
}

// 취소(초기 화면)
function btnCancel() {
    location.reload();
}

// 비밀번호 검사(3회 이상 비밀번호 오류 시 계좌정지 후 안내메시지 출력)
function checkPasswordError(acc_number, acc_password) {
    var value;
    $.ajax({
        type: "POST",
        url: "/checkPasswordError",
        async: false,
        data: {
            accNumber_give: acc_number,
            password_give: acc_password
        },
        success: function (response) {
            var real_password = response[0]['acc_password'];
            var input_password = response[1]['receive_password'];

            if (real_password == -1) {
                value = -1;
            } else if (real_password != input_password) {
                value = 1;
            } else {
                value = 0;
            }
        }
    })
    return value;
}

// 현재 계좌에 저장된 error 횟수 반환
function getErrorCount(bank_name, acc_number) {
    var currentError;
    $.ajax({
        type: "GET",
        url: "/getError",
        async: false,
        data: {
            bankName_give: bank_name,
            accNumber_give: acc_number
        },
        success: function (response) {
            currentError = response['error'];
        },
        error: function (e) {
            //alert('계좌 비밀번호 오류 횟수를 가져오지 못했습니다.');
        }
    });
    return currentError;
}

// 계좌에 저장된 error 횟수 초기화
function initError(acc_number) {
    var error_return;
    $.ajax({
        type: "POST",
        url: "/initPasswordError",
        async: false,
        data: {
            accNumber_give: acc_number
        },
        success: function (response) {
        },
        error: function (e) {
            error_return = 1;
        }
    })
    return error_return;
}

// DB에 저장된 계좌 정보 보기
function viewAccInfo() {
    $.ajax({
        type: "GET",
        url: "/viewAccInfo",
        async: false,
        data: {},
        success: function (response) {
            var bank_name = [];
            var client_name = [];
            var acc_number = [];
            var client_info = [];
            var masking_client_name = [];

            for (var i = 0; i < response['acc_info'].length; i++) {
                client_name[i] = response['acc_info'][i]['name'];
                bank_name[i] = response['acc_info'][i]['bankName'];
                acc_number[i] = response['acc_info'][i]['accNumber'];

                // 고객 이름을 마스킹 저장
                masking_client_name[i] = maskingName(client_name[i]);

                client_info[i] = [masking_client_name[i], bank_name[i], acc_number[i]];
            }
            alert(client_info.join('\n'));
        },
        error: function (e) {
            alert('계좌 정보를 가져오지 못했습니다.');
        }
    });
}

// 문자열 검색해서 중간 글자 *로 만들기(2글자일 경우 마지막 글자만)
function maskingName(strName) {
    if (strName.length > 2) {
        var originName = strName.split('');
        originName.forEach(function (name, i) {
            if (i === 0 || i === originName.length - 1) return;
            originName[i] = '*';
        });
        var joinName = originName.join();
        return joinName.replace(/,/g, '');
    } else {
        var pattern = /.$/; // 정규식
        return strName.replace(pattern, '*');
    }
}