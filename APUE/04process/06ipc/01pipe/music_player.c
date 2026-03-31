/* 
 * 多功能音乐播放器 - 修正版
 * 编译: gcc -o music_player music_player.c
 * 使用: ./music_player "歌曲1.mp3"
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_PLAYLIST 100

typedef struct {
    FILE *ctrl_pipe;
    char playlist[MAX_PLAYLIST][512];
    int current_song;
    int song_count;
    int volume;
    int running;
} player_state_t;

player_state_t state;

void send_command(const char *cmd)
{
    if(state.ctrl_pipe != NULL && cmd != NULL)
    {
        fprintf(state.ctrl_pipe, "%s", cmd);
        fflush(state.ctrl_pipe);
    }
}

void play_song(int index)
{
    if(index < 0 || index >= state.song_count) return;
    
    // 关闭当前播放
    if(state.ctrl_pipe != NULL)
    {
        fprintf(state.ctrl_pipe, "quit\n");
        fflush(state.ctrl_pipe);
        usleep(200000);
        pclose(state.ctrl_pipe);
        state.ctrl_pipe = NULL;
    }
    
    state.current_song = index;
    printf("\n🎵 正在播放: %s\n", state.playlist[state.current_song]);
    
    // 构建命令 - 使用简单的 popen 方式
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), 
             "mplayer \"%s\" -slave -quiet -volume %d 2>&1 | cat > /dev/null",
             state.playlist[state.current_song], state.volume);
    
    state.ctrl_pipe = popen(cmd, "w");
    if(state.ctrl_pipe == NULL)
    {
        perror("启动播放器失败");
        return;
    }
    
    // 等待启动
    usleep(300000);
    printf("✅ 播放中\n");
}

void next_song(void)
{
    if(state.song_count == 0) return;
    printf("\n⏭️ 下一曲\n");
    play_song((state.current_song + 1) % state.song_count);
}

void prev_song(void)
{
    if(state.song_count == 0) return;
    printf("\n⏮️ 上一曲\n");
    play_song((state.current_song - 1 + state.song_count) % state.song_count);
}

void volume_up(void)
{
    state.volume += 10;
    if(state.volume > 100) state.volume = 100;
    char cmd[50];
    sprintf(cmd, "volume %d 1\n", state.volume);
    send_command(cmd);
    printf("🔊 音量: %d%%\n", state.volume);
}

void volume_down(void)
{
    state.volume -= 10;
    if(state.volume < 0) state.volume = 0;
    char cmd[50];
    sprintf(cmd, "volume %d 1\n", state.volume);
    send_command(cmd);
    printf("🔉 音量: %d%%\n", state.volume);
}

void toggle_pause(void)
{
    if(state.ctrl_pipe == NULL) {
        printf("❌ 播放器未运行\n");
        return;
    }
    send_command("pause\n");
    printf("⏯️ 暂停/继续\n");
}

void seek_forward(void)
{
    if(state.ctrl_pipe == NULL) {
        printf("❌ 播放器未运行\n");
        return;
    }
    send_command("seek +10\n");
    printf("⏩ 快进 +10秒\n");
}

void seek_backward(void)
{
    if(state.ctrl_pipe == NULL) {
        printf("❌ 播放器未运行\n");
        return;
    }
    send_command("seek -10\n");
    printf("⏪ 快退 -10秒\n");
}

void show_help(void)
{
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║         🎵 音乐播放器控制 🎵           ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║  p     : 播放/暂停                     ║\n");
    printf("║  n     : 下一曲                        ║\n");
    printf("║  b     : 上一曲                        ║\n");
    printf("║  +     : 音量增加                      ║\n");
    printf("║  -     : 音量减小                      ║\n");
    printf("║  f     : 快进 10 秒                    ║\n");
    printf("║  r     : 快退 10 秒                    ║\n");
    printf("║  1-9   : 选择歌曲                      ║\n");
    printf("║  l     : 显示播放列表                  ║\n");
    printf("║  s     : 显示状态                      ║\n");
    printf("║  h     : 显示帮助                      ║\n");
    printf("║  q     : 退出                          ║\n");
    printf("╚════════════════════════════════════════╝\n");
}

void show_playlist(void)
{
    printf("\n========== 播放列表 ==========\n");
    for(int i = 0; i < state.song_count; i++)
    {
        printf("%s%d. %s\n", 
               (i == state.current_song) ? "▶ " : "  ",
               i+1, state.playlist[i]);
    }
    printf("==============================\n");
}

void show_status(void)
{
    printf("\n========== 播放状态 ==========\n");
    printf("当前歌曲: %s\n", state.playlist[state.current_song]);
    printf("歌曲序号: %d/%d\n", state.current_song+1, state.song_count);
    printf("音    量: %d%%\n", state.volume);
    printf("==============================\n");
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("用法: %s \"歌曲文件1.mp3\" [歌曲文件2.mp3] ...\n", argv[0]);
        return 1;
    }
    
    // 初始化
    memset(&state, 0, sizeof(state));
    state.volume = 70;
    state.running = 1;
    state.ctrl_pipe = NULL;
    
    // 添加歌曲
    printf("\n添加歌曲:\n");
    for(int i = 1; i < argc && state.song_count < MAX_PLAYLIST; i++)
    {
        if(access(argv[i], R_OK) == 0)
        {
            strcpy(state.playlist[state.song_count], argv[i]);
            state.song_count++;
            printf("  ✓ %s\n", argv[i]);
        }
        else
        {
            printf("  ✗ 无法访问: %s\n", argv[i]);
        }
    }
    
    if(state.song_count == 0)
    {
        printf("\n错误: 没有有效的歌曲文件\n");
        return 1;
    }
    
    printf("\n共 %d 首歌曲\n", state.song_count);
    show_playlist();
    play_song(0);
    show_help();
    
    // 主循环
    char input[10];
    while(state.running)
    {
        printf("\n🎮 命令: ");
        fflush(stdout);
        
        if(fgets(input, sizeof(input), stdin) == NULL)
            break;
        
        input[strcspn(input, "\n")] = 0;
        if(strlen(input) == 0) continue;
        
        char cmd = tolower(input[0]);
        
        switch(cmd)
        {
            case 'p': toggle_pause(); break;
            case 'n': next_song(); break;
            case 'b': prev_song(); break;
            case '+': volume_up(); break;
            case '-': volume_down(); break;
            case 'f': seek_forward(); break;
            case 'r': seek_backward(); break;
            case 'l': show_playlist(); break;
            case 's': show_status(); break;
            case 'h': show_help(); break;
            case 'q': 
                state.running = 0;
                printf("\n👋 再见！\n");
                break;
            default:
                if(cmd >= '1' && cmd <= '9')
                {
                    int idx = cmd - '1';
                    if(idx < state.song_count)
                        play_song(idx);
                    else
                        printf("歌曲 %d 不存在\n", idx+1);
                }
                else
                {
                    printf("未知命令: %c (h=帮助)\n", cmd);
                }
                break;
        }
    }
    
    // 清理
    if(state.ctrl_pipe)
    {
        fprintf(state.ctrl_pipe, "quit\n");
        fflush(state.ctrl_pipe);
        usleep(200000);
        pclose(state.ctrl_pipe);
    }
    
    return 0;
}
