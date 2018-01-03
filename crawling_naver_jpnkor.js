var request=require("sync-request");
var fs = require("fs");

//var url="http://jpdic.naver.com/entry/jk/JK000000024161.nhn";
var content_blk,
    content_start_pos, content_end_pos,
    title_str,
    title_start, title_end,
    conjugation_blk,
    conjugation_start, conjugation_end,
    meaning_blk,
    meaning_start, meaning_end,
    temp_blk;

var start_index=1;  // 일한사전 (JK) : i<=303350  (total:  0 ~ 303350)
                      // 한일사전 (KJ) : i<=98162  (total:  0 ~ 98162)
                      // 한자어 사전에 대해서는 아직 no-idea.
fs.writeFileSync("jkdic.txt", ' \n', 'utf8');   // to Clear latest file.
for (var i=start_index; i<start_index+303351; i++) {     // 시스템(서버)의 용량에 따라 다르겠지만, 내 PC 에서는 한번에 300개 정도가 최대 값이다. 그 이상 되면 먹통이 되더라..

      // {  var i=67848;
  (function(i) {     // 이것은 단지 i 라는 값을 callback 내에서 알맞게 쓰기 위한 방법일 뿐.
    var zerofilled_id = ('000000000000'+i).slice(-12);
    var url="http://jpdic.naver.com/entry/jk/JK"+ zerofilled_id + ".nhn";

    try {
      var res = request('GET', url);
      var html = (String)(res.getBody());
      // request('GET', url, function(error, response, html) {

        // if ( error != null) {
        //   console.log(error);
        //   return;
        // }
        //console.log(html);
            // <!-- content -->  블럭을 추출.
        content_start_pos = html.indexOf('<!-- content -->') + 16;
        content_end_pos = html.indexOf('<!-- //content -->');
        content_blk = html.substring(content_start_pos, content_end_pos);

        conjugation_start = content_blk.indexOf('<!-- conjugation -->') + 20;      // '<!-- conjugation -->'
        if (conjugation_start > 0) {   // 존재한다면.
          conjugation_end = content_blk.indexOf('<!-- //conjugation -->');       // '<!-- //conjugation -->'
          conjugation_blk = content_blk.substring(conjugation_start, conjugation_end);
        } else {
          conjugation_blk = "";
        }

            // <!-- content -->  블럭을 추출.
        meaning_start = content_blk.indexOf('<!-- meaning_body -->') + 21;
        meaning_end = content_blk.indexOf('<!-- //meaning_body -->');
        meaning_blk = conjugation_blk + content_blk.substring(meaning_start, meaning_end);
        temp_blk = meaning_blk.replace(/\t/g, " ");
        meaning_blk = temp_blk.replace(/\n/g, "");

        conjugation_blk = null;   // 이제 버퍼는 불필요 함.  메모리 확보를 위해 날려 버림.
          /* 추가로 해야 할 일 들..
            1. 중복된 SPACE 들을 정리. ==> <h5>                접미사       </h5>  이렇게 쓸데 없이 많은 SPACE 정리.
            2. "lst mean_level_1 kr_lst ""   <-- 여기 끝에 쌍따옴표 닫는거 이상함. 이것도 고칠 것.
                //// 3. "display:none"  의 문자열을 "display:block" 으로 변경., <span class="blind">동의어</span> 의 문자열 제거.
            4. [발음재생] 버튼 삭제 하고, [예문보기] 버튼 삭제하고.
            5. <a href> </a> 링크 모두 삭제.
          */

          ////   3. "display:none"  의 문자열을 "display:block" 으로 변경., <span class="blind">동의어</span> 의 문자열 제거.
          temp_blk = meaning_blk.replace(/display:none/g, "display:block");
          meaning_blk = temp_blk.replace(/동의어/g, " ");


        content_start_pos = content_blk.indexOf('<div id="content"');
        if ( (content_start_pos < 0) || (content_start_pos >= 30) )  {   // Not found. or 'weird data'
          console.log( "WEIRD DATA : " + url);
          return;
        }

        title_start = content_blk.indexOf('class="maintitle">') + 18;
        temp_blk = content_blk.substr(title_start,1000);
        title_end = temp_blk.indexOf('</span>');
        title_str = content_blk.substr(title_start, title_end);

        fs.appendFileSync("jkdic.txt", title_str + "\t" + meaning_blk + '\n', 'utf8');
              //console.log ("start:" + content_start_pos + ", end:" + content_end_pos + "\n");
              console.log (zerofilled_id + ", " + url + " : " + title_str);
              // console.log(title_str + "\t" + meaning_blk);

    } catch (e)
    {
      console.log(e);
    }
    // });
  })(i);

}
