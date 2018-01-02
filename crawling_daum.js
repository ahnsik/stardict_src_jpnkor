var request=require("request");

var url="http://alldic.daum.net/word/view.do?wordid=jkw000083585&suptype=KOJIEN";
var subject_blk,   // 표제어 블럭
    subject_start_pos,  // 표제어 블럭의 시작 위치
    subject_end_pos,    // 표제어 블럭의 길이
    descript_blk,       // 설명부 블럭
    descript_start_pos, // 설명부 블럭의 시작 위치
    descript_end_pos,   // 설명부 블럭의 길이.
    temp_blk;

request(url, function(error, response, html) {
  //console.log(html);
  subject_start_pos = html.indexOf('class="detail_top">') + 19;
  subject_end_pos = html.indexOf('<div id="mArticle">');
  subject_blk = html.substring(subject_start_pos, subject_end_pos);
  subject_start_pos = subject_blk.indexOf('class="txt_cleanword">') + 22;
  temp_blk = subject_blk.substr(subject_start_pos,1000);
  subject_end_pos = temp_blk.indexOf('<');
  subject_blk = temp_blk.substring(0, subject_end_pos);

  descript_start_pos = html.indexOf('class="box_word">') + 17;
  descript_end_pos = html.indexOf('<div id="families" class="card_tit">');
  descript_blk = html.substring(descript_start_pos, descript_end_pos);

  console.log(subject_blk + "\n-=-=-=-=-=-=-=-=-=-\n" + descript_blk );
});
