#include "steer.hpp"
#include "car.hpp"
#include "car_config.h"
#include "communication.hpp"
#include "deep.hpp"
#include "gui.hpp"
#include "magnet.hpp"
#include "package.hpp"
#include "route.h"
#include "util.h"
#include "zf_qtimer.h"
#include <string>

//-------------------------------------------------------------------------------------------------------------------
//  @name
//  闂傚倸鍊搁崐鎼佸磹閹间礁纾圭€瑰嫭鍣磋ぐ鎺戠倞鐟滃寮搁弽顓熺厸闁搞儯鍎遍悘鈺冪磼閹邦収娈滈柡宀€鍠栭幃褔宕奸悢鍝勫殥闂備礁鎲￠崹瑙勬叏閹绢喖鐓橀柟杈鹃檮閸嬫劙鎮楅崷顓炐㈡い銉︾箘缁辨挻鎷呴崫鍕碘偓宀勬煕閵娿儳鍩ｇ€殿喖顭烽弫鎰緞婵犲嫷鍚呴梻浣瑰缁诲倿骞夊☉銏犵缂備焦顭囬崢鎼佹⒑閸涘﹣绶遍柛鐘愁殜瀹曟劙骞囬悧鍫㈠幐閻庡厜鍋撻悗锝庡墰閻﹀牓鎮楃憴鍕閻㈩垱甯熼悘鎺楁煟閻愬鈻撻柍褜鍓欓崢鏍ㄧ珶閺囥垺鈷戦柛婵嗗閳诲鏌涚€Ｑ冧壕闂備胶枪椤戝棝骞愰幖渚婄稏婵犻潧顑愰弫鍡涙煕鐏炲墽鏁栭柕濞炬櫆閳锋垿鏌涘┑鍡楊伂妞ゎ偓绠撻弻娑㈠籍閳ь剟宕归崸妤冨祦闁告劦鍠栭～鍛存煏閸繃鍣圭紒鎰⊕缁绘繈鎮介棃娴躲垽鎮楀闂寸敖婵″弶鍔欏鎾閳锯偓閹风粯绻涙潏鍓у埌闁硅姤绮庣划鏃堟倻濡晲绨婚梺闈涱檧闂勫嫬鐣峰畝鈧埀顒冾潐濞叉﹢銆冮崨杈剧稏婵犲﹤鐗嗛悞鍨亜閹哄棗浜惧銈嗘穿缂嶄線銆侀弴銏℃櫇闁逞屽墰缁鎳￠妶鍥╋紳婵炶揪缍€椤曟牠鎮炴禒瀣厱婵☆垵顕ф慨宥嗘叏婵犲倹鎯堥弫鍫ユ煕閵夋垵鎳嶉惀顏呬繆閻愵亜鈧牕顫忚ぐ鎺撳亱闁绘ê妯婇崵鏇㈡煕閹邦厾銈撮柛顐邯閺屾盯顢曢妶鍛亖闂佸憡锚閻忔碍绌辨繝鍥ㄥ€风€瑰壊鍠栭崜鐗堜繆閵堝洤校闁诡喖鍊搁悾鐑藉閵堝懐顔掑銈嗘⒒閺咁偊宕㈤崨濠勭閺夊牆澧界粔顒併亜椤愩埄妲哄ù婊勬倐閹晫鍠婂Ο杞板闂佸壊鐓堥崑鍛閺屻儲鐓ラ柡鍥ュ妺缁ㄥ鏌ｉ敐鍥у幋鐎殿喖顭锋俊鐑藉Ψ椤旂晫绉煎┑鐘垫暩婵炩偓婵炰匠鍥舵晞闁糕剝绋戦悡鏇㈡煙鏉堥箖妾柛瀣剁秮閺岀喖顢橀悢椋庣懆婵犵鈧偨鍋㈤柡宀嬬磿閳ь剨缍嗛崑鍕倶閹绢喗鐓涚€光偓鐎ｎ剛锛熼梺閫炲苯澧剧紓宥呮缁傚秹宕奸弴鐔蜂簵闂佺ǹ绻掗埛鍫濃枔娴犲鐓熼柟閭﹀灠閻ㄨ櫣绱掗悩宸吋闁哄瞼鍠栭、娆撴偩瀹€鈧悡澶愭⒑閻熸壆锛嶉柛瀣ㄥ€栨穱濠囨倻閼恒儲娅嗙紓浣圭☉椤戝嫰骞嬮悜鑺モ拻闁稿本鑹鹃埀顒傚厴閹虫宕滄担绋跨亰濡炪倖鐗滈崑鐐哄磻閿熺姴绾ч柛顐ｇ濞呭棙銇勯锝嗙闁哄矉绻濆畷鍫曞Ψ閵壯冨Ф闂備浇妗ㄧ粈浣肝涘┑瀣摕闁挎稑瀚▽顏堟偣閸ャ劌绲诲┑顔兼捣缁辨挻鎷呴崫鍕碘偓宀勬煕閵婏箑鈻曢柛鈹惧亾濡炪倖甯掗崐褰掑吹閳ь剟鏌ｆ惔銏㈢叝闁告鍟块悾鐑藉箣閻愮數鐦堥梺绋胯閸婃宕濆畡鎵虫斀閹烘娊宕愰弴銏犵疇闊洦绋戦悡鏇㈡煙鏉堥箖妾柛瀣剁秮閺岀喖顢橀悢椋庣懆婵犵鈧偨鍋㈤柡宀嬬磿閳ь剨缍嗛崑鍕倶閹绢喗鐓涚€光偓鐎ｎ剛锛熼梺閫炲苯澧剧紓宥呮缁傚秹宕奸弴鐔蜂簵闂佺ǹ绻掗埛鍫濃枔娴犲鐓熼柟閭﹀灠閻ㄨ櫣绱掗悩宸吋闁哄备鍓濋幏鍛喆閸曨偆锛撻梻浣芥〃閻掞箓骞戦崶顒€鏋侀柟鍓х帛閺呮悂鏌ㄩ悤鍌涘
//  @sample   Motor
//  MotorL(MOTOR_L,encoderL_QTIMER,encoderL_LSB_QTIMER,encoderL_DIR_QTIMER);
//  @sample   Motor
//  MotorR(MOTOR_R,encoderR_QTIMER,encoderR_LSB_QTIMER,encoderR_DIR_QTIMER);
//-------------------------------------------------------------------------------------------------------------------
Steer::Steer(PWMCH_enum ch, float OutMin, float OutMax)
    : PWM_Channel(ch), steerCtrl(std::make_pair(OutMin, OutMax)),
      steerOffset(0) {}

void Steer::Init() {
    pwm_init(PWM_Channel, STEER_FREQ,
             PERCENT_TO_TICKS(PULSE_TO_PERCENT(STEER_CENTER)));
}

void Steer::DutySet(float duty) {
    CAR_ERROR_CHECK(PERCENT_TO_TICKS(duty) < PWM_DUTY_MAX);
    pwm_duty(PWM_Channel, PERCENT_TO_TICKS(duty));
}

//-------------------------------------------------------------------------------------------------------------------
//  @name
//  pid闂傚倸鍊搁崐鎼佸磹閹间礁纾圭€瑰嫭鍣磋ぐ鎺戠倞鐟滃寮搁弽顓熺厸闁搞儯鍎遍悘鈺冪磼閹邦収娈滈柡宀€鍠栭幃褔宕奸悢鍝勫殥闂備礁鎲￠崹瑙勬叏閹绢喖鐓橀柟杈鹃檮閸嬫劙鎮楅崷顓炐㈡い銉︾箘缁辨挻鎷呴崫鍕碘偓宀勬煕閵娿儳鍩ｇ€殿喖顭烽弫鎰緞婵犲嫷鍚呴梻浣瑰缁诲倿骞夊☉銏犵缂備焦顭囬崢鎼佹⒑閸涘﹣绶遍柛鐘愁殜瀹曟劙骞囬悧鍫㈠幐閻庡厜鍋撻悗锝庡墮閸╁矂姊虹€圭媭鍤欑紒澶婄秺閵嗕線寮介鐐电暰閻熸粌顦靛畷鎴﹀箻鐠囧弬褍顭跨捄鐚村姛闁哄苯鐗撳娲箹閻愭彃濮堕梺鍛婃尰瀹€鎼佸箖閳ユ枼鏋庨煫鍥风稻鐎靛矂鏌ｆ惔顖滅У濞存粠浜、鎾澄旈崨顔惧幈濠碘槅鍨崇划顖滀焊閿曞倹鐓涢悘鐐登规晶鏌ユ煙瀹勭増鍣介柟鍙夋尦瀹曠喖顢曢妶蹇曞闂傚倸鍊峰ù鍥ь浖閵娾晜鍊块柨鏇炲€哥粻鏌ユ煕閵夘喖澧柡瀣╃窔閺岀喖宕滆鐢盯鏌￠崨顔藉€愰柡灞诲姂閹倝宕掑☉姗嗕紦
//-------------------------------------------------------------------------------------------------------------------
void steer_schedule(sched_event_data_t dat) {
    float steering;
    if (Car.CtrlMode == ControlMode::AI)
        steering = deep_predict();
    else {
        steering =
            Car.Steer3010.steerCtrl.Realize(MagErrorForPID) + STEER_CENTER;
        LIMITING(steering, STEER_MIN, STEER_MAX);
        
        package_sendDeep(
            RESCALE_VALUE(steering - STEER_MIN, 255, STEER_MAX - STEER_MIN));
    }
    Car.Steer3010.WidthSet(steering);
    Car.MotorL.target = Car.MotorR.target =
        Car.TargetSpeed - std::abs(steering - STEER_CENTER) * Car.Deceleration;
}
