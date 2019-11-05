.PHONY: clean All

All:
	@echo "----------Building project:[ FinalProject - Debug ]----------"
	@cd "lib" && "$(MAKE)" -f  "FinalProject.mk"
clean:
	@echo "----------Cleaning project:[ FinalProject - Debug ]----------"
	@cd "lib" && "$(MAKE)" -f  "FinalProject.mk" clean
