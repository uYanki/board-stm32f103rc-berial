module main(
    input clk_i,
    input nrst_i
);
    reg [23:0] cnt;

    reg ram_clk;

    initial begin
        cnt <= 24'd0;
        ram_clk <= 1'b0;
    end

    always @(posedge clk_i or negedge nrst_i) begin
        if(!nrst_i) begin
            cnt <= 24'd0;
            ram_clk <= 1'b0;
        end else begin
            if(cnt == 24'd10) begin
                cnt <= 24'd0;
                ram_clk <= !ram_clk;
            end else begin
                cnt <= cnt + 1'b1;
            end
        end    
    end

    wire dat;

    ram ram_test(
        .cs_i(1'b0),
        .sclk(ram_clk),
        .din(1'b0),
        .dout(dat)
    );

endmodule