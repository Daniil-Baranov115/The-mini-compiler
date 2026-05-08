; ModuleID = 'mini_compiler'
source_filename = "mini_compiler"
target triple = "aarch64-unknown-linux-gnu"

define i64 @compiled_fn(i64 %arg) {
entry:
  %result = alloca i64, align 8
  %arg1 = alloca i64, align 8
  %return.value = alloca i64, align 8
  store i64 0, ptr %return.value, align 4
  store i64 %arg, ptr %arg1, align 4
  store i64 0, ptr %result, align 4
  %arg.load = load i64, ptr %arg1, align 4
  switch i64 %arg.load, label %switch.default [
    i64 1, label %switch.case.1
    i64 2, label %switch.case.2
    i64 3, label %switch.case.3
  ]

compiled_fn.exit:                                 ; preds = %switch.end
  %result2 = load i64, ptr %return.value, align 4
  ret i64 %result2

switch.end:                                       ; preds = %switch.default, %switch.case.3, %switch.case.2, %switch.case.1
  %result.load = load i64, ptr %result, align 4
  store i64 %result.load, ptr %return.value, align 4
  br label %compiled_fn.exit

switch.default:                                   ; preds = %entry
  store i64 0, ptr %result, align 4
  br label %switch.end

switch.case.1:                                    ; preds = %entry
  store i64 10, ptr %result, align 4
  br label %switch.end

switch.case.2:                                    ; preds = %entry
  store i64 20, ptr %result, align 4
  br label %switch.end

switch.case.3:                                    ; preds = %entry
  store i64 30, ptr %result, align 4
  br label %switch.end
}
