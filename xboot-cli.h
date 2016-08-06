#ifndef __XBOOT_CLI_H__
#define __XBOOT_CLI_H__

#define CLI_TIMEOUT_MS (500)

// Check if a client wants us to enter the CLI by sending a stream of ':'
bool enter_cli(void);
// Run the CLI
void start_cli(void);


#endif // __XBOOT_CLI_H__
